#include "cachelab.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
/*
应该是只有一级缓存 L M S的具体含义见下
这是通过查 https://www.valgrind.org/docs/manual/valgrind_manual.pdf
然后发现valgrind --log-fd=1 --tool=lackey -v --trace-mem=yes ./csim 指令的具体含义 
告诉我需要在lackey/lk_main.c文件的comment中找输出的format含义 于是我就到github上找了对应的表述 如下
*/
/*
It prints memory data access traces that look like this:

  I  0023C790,2  # instruction read at 0x0023C790 of size 2
  I  0023C792,5
   S BE80199C,4  # data store at 0xBE80199C of size 4
  I  0025242B,3
   L BE801950,4  # data load at 0xBE801950 of size 4
  I  0023D476,7
   M 0025747C,1  # data modify at 0x0025747C of size 1
  I  0023DC20,2
   L 00254962,1
   L BE801FB3,1
  I  00252305,1
   L 00254AEB,1
   S 00257998,1

Every instruction executed has an "instr" event representing it.
Instructions that do memory accesses are followed by one or more "load",
"store" or "modify" events.  Some instructions do more than one load or
store, as in the last two examples in the above trace.

Here are some examples of x86 instructions that do different combinations
of loads, stores, and modifies.

   Instruction          Memory accesses                  Event sequence
   -----------          ---------------                  --------------
   add %eax, %ebx       No loads or stores               instr

   movl (%eax), %ebx    loads (%eax)                     instr, load

   movl %eax, (%ebx)    stores (%ebx)                    instr, store

   incl (%ecx)          modifies (%ecx)                  instr, modify

   cmpsb                loads (%esi), loads(%edi)        instr, load, load

   call*l (%edx)        loads (%edx), stores -4(%esp)    instr, load, store
   上边这个就是先把(%edx)load到%rip中，然后再把返回地址压栈
   pushl (%edx)         loads (%edx), stores -4(%esp)    instr, load, store
   movsw                loads (%esi), stores (%edi)      instr, load, store

Instructions using x86 "rep" prefixes are traced as if they are repeated
N times.

经过上边的演示，可以知道L表示load，CPU从内存中读数据到寄存器 需要经过缓存（这里假设缓存只有一级） 需要考虑命中和不命中的情况
S表示store，CPU将寄存器中的数据储存到内存中，仍然需要经过缓存(也就是写数据到内存中，涉及到写回和写分配)
M表示先Load再Store
*/
int s,E,b;
FILE * fp;
int flaginfo = 0;
typedef unsigned long addrType;
addrType Time = 0; //模拟时间，block每存储到缓存器中Time++
const addrType MaxTime = 0xffffffffffffffff;

void accTime() //数据封装 见到该数据的函数越少越好
{
    Time++;
}
addrType readTime()
{
    return Time;
}

int sizeStr(char * str)
{
    int i=0;
    while(str[i]!='\0')
        i++;
    return i;
}

unsigned int decString2uint(char * str) //正十进制字符串到unsigned int 
{
    int size = sizeStr(str);
    unsigned int sum = 0;
    char currentchar;
    for(int i=1;i<=size;i++)
    {
        currentchar = str[i-1];
        if(currentchar>='0'&&currentchar<='9')
        {
            sum += pow(10,size-i) * (currentchar-'0');
        }
        else
        {
            return -1; // 读取失败
        }
    }
    return sum;
}

bool isLetter(char c) //判断一个字符是不是字母
{
    if ((c>='a'&&c<='z') || (c>='A'&&c<='Z'))
        return 1;
    else 
        return 0;
}

char toLower(char c) //大写字母转化为小写字母
{
    if (c>='A'&&c<='Z')
        return c-'A'+'a';
    else if (c>='a'&&c<='z')
        return c;
    else
        return '\0'; //不是字母 不能处理这种情况
}

addrType hexString2uintAddr(char * str) //正的十六进制字符串转化到unsigned int储存的地址格式 也就是一个16进制对应4个二进制
//不能直接转换为十进制！！！！
{
    int size = sizeStr(str);
    addrType sum = 0;
    char currentchar;   
    int i = 0;
    int count = 1;
    if (size>=2&&(str[1]=='x'||str[1]=='X'))
    {
        i = 2;
        size = size-2;
    }
        
    while(str[i]!='\0')
    {
        currentchar = isLetter(str[i])?toLower(str[i]):str[i];
        if (currentchar>='a'&&currentchar<='f')
            //sum += pow(16,size-(count++)) * (10+(currentchar-'a')); //这是直接转化成十进制了 按照位的观点是不能的！！！
            sum |= ((addrType)(10+(currentchar-'a')))<<(4*(size-(count++)));
        else if (currentchar>='0'&&currentchar<='9')
            //sum += pow(16,size-(count++)) * (currentchar-'0');
            sum |= ((addrType)(currentchar-'0'))<<(4*(size-(count++)));
        else 
            return -1;
        i++;
    }
    return sum;
}

typedef struct lineData
{
    int operatrion;
    addrType addr;
}lineData;
const int opErr = -1;
const int opI = 0;
const int opL = 1;
const int opS = 2;
const int opM = 3;

char * skipSpace(char * beg)
{
    while(*beg!='\0'&&*beg==' ')
        beg++;
    return beg;
}

lineData analyseMemoryLine(char * line)
{
    int i = 0;
    lineData result;
    result.addr = 0;
    result.operatrion = opErr;

    line = skipSpace(line);

    if (line[i]=='I')
        result.operatrion = opI;
    else if (line[i]=='S')
        result.operatrion = opS;
    else if (line[i]=='L')
        result.operatrion = opL;
    else if (line[i]=='M')
        result.operatrion = opM;
    line++;

    line = skipSpace(line);
    char addr[100];
    char * ptr = addr;
    while(*line !=',')
        *(ptr++) = *(line++); 
    *ptr = '\0';
    result.addr = hexString2uintAddr(addr);

    return result;
}

void analyseInputData(int argc,char * argv[])
{
    for(int i =0;i<argc;i++)
    {
        if (argv[i][1]=='v')
            flaginfo = 1;
        else if (argv[i][1]=='s')
        {
            i++;
            s = decString2uint(argv[i]);
        }
        else if (argv[i][1]=='E')
        {
            i++;
            E = decString2uint(argv[i]);
        }
        else if (argv[i][1]=='b')
        {
            i++;
            b = decString2uint(argv[i]);
        }
        else if (argv[i][1]=='t')
        {
            i++;
            fp = fopen(argv[i],"r");
        }
    }
}

typedef struct Index
{
    addrType GroupNum; //地址对应的组号
    addrType Mark; //地址对应的组中的标记(也就是行)
    addrType offset;  //地址对应的组、行的偏移字节

}Index;

Index analyseAddr(lineData ld) 
//解析地址 得到应该访问的索引
{
    Index result;
    unsigned char m = sizeof(ld.addr)*8;
    result.offset = (ld.addr<<(m-b))>>(m-b);
    result.GroupNum = ((ld.addr>>b)<<(m-s))>>(m-s);
    result.Mark = ld.addr>>(s+b);
    return result;
}

typedef struct cacheline //数据块所在的行
{
    bool valid ; //有效位
    addrType mark; //标记
    char * block; //块的起始地址 块的字节大小为2^b
    addrType TimeStamp;
}cacheline;
typedef struct cache //一共有2^s个组 每组有E行  因此一共有E*2^s行cacheline
{
    cacheline * cachebeg;
}cache;

void initcache(cache * cacheUsed)
{
    addrType linesNum = E*pow(2,s);
    cacheUsed->cachebeg = (cacheline *)malloc(linesNum*sizeof(cacheline));
    for (addrType i = 0;i<linesNum;i++)
    {
        cacheUsed->cachebeg[i].valid = 0;
        cacheUsed->cachebeg[i].block = 0;
        cacheUsed->cachebeg[i].mark = 0;
        cacheUsed->cachebeg[i].TimeStamp = readTime();
    }
}

typedef struct visitTypeCount
{
    addrType hit;
    addrType miss;
    addrType eviction;
}visitTypeCount;

void initvisitTypeCount(visitTypeCount* vCount)
{
    vCount->hit = 0;
    vCount->miss = 0;
    vCount->eviction = 0;
}

const int hit_state = 0;
const int miss_state = 1;
const int eviction_state = 2;
const int miss_eviction_state = 3;

void strcopy(const char*src,char*dst)
{
    while(*src!='\0')
    {
        *(dst++) = *(src++);
    }
    *dst = '\0';
}

void trans2String(const int state,char * str)
{
    if (state == hit_state)
        strcopy("hit ",str);
    else if (state == miss_state)
        strcopy("miss ",str);
    else if (state==eviction_state)
        strcopy("eviction ",str);
    else if (state==miss_eviction_state)
        strcopy("miss_eviction ",str);
    else
        strcopy("",str);
}

int cacheLogic(cache * cacheUsed,const Index inp,visitTypeCount *vCount) 
//缓存逻辑 Index可能是S的访问 也可能是L的访问 由于写是写回和写分配，因此和L是一样的M是先L再S
//实现读写的特定功能就在下边的注释区域写 已经用伪代码写了
//可以理解为：多个本函数互相通信构成了多级缓存的存储结构
{
    accTime();
    const addrType lineoffset = inp.GroupNum*E;
    int countValid = 0;
    addrType minTimeStamp = MaxTime;
    int mini = -1;
    int Invalidi = -1;
    for(int i = 0;i<E;i++)
    {
        if (cacheUsed->cachebeg[lineoffset+i].valid==1)
        {
            countValid++;

            if (cacheUsed->cachebeg[lineoffset+i].TimeStamp<minTimeStamp)
            {
                minTimeStamp = cacheUsed->cachebeg[lineoffset+i].TimeStamp;
                mini = i;
            }
            
            if (cacheUsed->cachebeg[lineoffset+i].mark == inp.Mark)
            {
                vCount->hit++;
                cacheUsed->cachebeg[lineoffset+i].TimeStamp = readTime(); //更新时间戳 每个缓存结构有一个自己的时间戳
                //命中！

                /*
                需要注意的是缓存之间传递的是块，而CPU与L1传递的直接是Offset的数据 下同 这里为了伪代码的清晰，就不写特别细节的东西了
                （不区分Offset和block data  实际上是要精细的区分的）
                各级缓存的块大小是一样的，都是64字节，只是组数和相连度不一样造成了存储空间的不一样 
                下边所述的block data都是64字节

                read是L write是S

                if read
                    return block data to higher cache
                else if write
                    write data from higher cache to block data
                    change ModifiedFlag to 1 //置位修改位

                */
                return hit_state;
            }
        }
        else
        {
            Invalidi = i;
        }
    }

    vCount->miss++; //说明没命中
    if (countValid==E) //说明已经满了且没有命中 需要根据时间戳驱逐 驱除最久未使用的数据值 以低层缓存数据替换
    {
        vCount->eviction++;
        cacheUsed->cachebeg[lineoffset+mini].TimeStamp = readTime();
        cacheUsed->cachebeg[lineoffset+mini].mark = inp.Mark;
        /*
            也就是说这一块负责将封装好的下一级返回的数据填入到block并把原来的block数据释放或者根据修改位将修改传递到下一级缓存（写回机制）
            最后根据offset将数据返回去或者 修改数据及置位修改位！！！！
            
            注意！！下一级缓存做什么，我这里并不关心，我只关心的是在读时我要从你下一级缓存中得到我想要的数据块，下一级缓存把数据块给我就行
            在写时我把写命令给下一级，不需要你返回数据块，你下一级缓存能把我给你的写命令及修改后的数据在下一级的缓存中的对应块中完成即可

        */

        /*
        if modified flag of cachebeg[lineoffset+mini] is 1 //写回机制
            block data write to lower cache 
            //如果修改位置1 则将修改过的数据block传递到低级cache 给下一级缓存发信号及数据 也可能会递归 因为下一级可能没有对应的块，还需要找
        else
            delete block data //原block空间没有修改过，释放本级缓存的对应内存
        //上边的是对原数据的处理

        if read
            read data from lower cache -> block data 
            //这可能会递归，因为下一级缓存可能没有想要的block data 既然是递归，那么完全封装好了，对本级缓存来说 我下边是有10级缓存还是1级都没有差别
            return block data to higher cache
        else if write
            read data from lower cache -> block data
            write data from higher cache to block data 
            change ModifiedFlag to 1 //置位修改位

        */
        return miss_eviction_state;
    }
    else //没满且没命中
    {
        cacheUsed->cachebeg[lineoffset+Invalidi].valid = 1;
        cacheUsed->cachebeg[lineoffset+Invalidi].TimeStamp = readTime();
        cacheUsed->cachebeg[lineoffset+Invalidi].mark = inp.Mark;
        /*
        if read
            read data from lower cache -> block data
            return block data to higher cache
        else if write
            read data from lower cache -> block data
            write data from higher cache to block data 
            change ModifiedFlag to 1 //置位修改位

        */
        return miss_state;
    }

}


int main(int argc,char * argv[]) 
//argv是储存char*的数组 整个main包括缓存结构的初始化到缓存的输入输出其实就可以看成是一级的缓存 然后可以有多个main构成多级缓存
{
    //printf("%ld",hexString2uintAddr("0x1Ab3"));
    // printf("%d",hexString2uint("1Ab3"));
    // printf("%d",decString2uint("13"));

    analyseInputData(argc,argv); //设置缓存器结构
    // printf("%d,%d,%d\r\n",E,s,b);
    // printf("%lud ||||||||||\r\n",MaxTime+1);
    // printf("%lud ||||||||||\r\n",MaxTime);

    cache cacheUsed;
    initcache(&cacheUsed); //设置缓存器结构
    visitTypeCount vCount;
    initvisitTypeCount(&vCount);
    
    char line[100];
    char str[50];
    while (fgets(line,100,fp)) //如果看成多级缓存的理解的话 就不要看这个循环 就考虑下边的缓存输入输出即可
    {
        if (flaginfo==1)
            printf("%s ",line);
        lineData thisd = analyseMemoryLine(line); //这相当于接收从上一级存储结构发送来的地址 
        Index thisindex = analyseAddr(thisd); //解析地址
        if (thisd.operatrion==opI) //不记录I指令的缓存情况
            continue;
        else if (thisd.operatrion==opL || thisd.operatrion==opS)
        {
            trans2String(cacheLogic(&cacheUsed,thisindex,&vCount),str); 
            //根据本级缓存结构和解析的地址看是否命中并返回数据 若不命中还负责向下层的存储结构要数据并返回 否则命中的话就直接返回
            //也相当于做了一个封装，上一级的缓存只管向下一级要，不管下一级怎么得到，反正上一级的缓存得到了从下一级返回的数据
            //下一级对于下下一级还是同样的道理 是一个递归 跟DNS差不多 
            //多个cacheLogic互相通信构成了多级缓存结构
            if (flaginfo==1)
                printf("%s ",str);
        }
            
        else if (thisd.operatrion==opM)
        {
            trans2String(cacheLogic(&cacheUsed,thisindex,&vCount),str);
            if (flaginfo==1)
                printf("%s ",str);
            trans2String(cacheLogic(&cacheUsed,thisindex,&vCount),str);
            if (flaginfo==1)
                printf("%s ",str);
        }
        // printf("%ld ",thisindex.GroupNum);
        // printf("%d ",thisd.operatrion);
        // printf("%ld \r\n",thisd.addr);
        if (flaginfo==1)
            printf("\r\n");
    }
    
    printSummary(vCount.hit, vCount.miss, vCount.eviction);
    return 0;
}
/*
多级缓存机制分析：

经过上述分析，多级缓存冷启动时，CPU如果想读取一个位于磁盘中的数据，那么L1找不到，L1让L2找，L2找不到，L2让L3找，L3找不到，L3让主存找
主存找不到，主存让磁盘找，然后磁盘找到了，将数据返回到主存中，主存缓存，并把数据返回到L3，L3缓存，并把数据返回到L2，L2缓存，并把数据
返回到L1，L1缓存，并把数据返回到CPU 

当写数据时，多级缓存冷启动时，CPU想写一个位于磁盘中的数据，那么L1找不到，L1让L2找，L2找不到，L2让L3找，L3找不到，L3让主存找
主存找不到，主存让磁盘找，然后磁盘找到了，将数据返回到主存中，主存缓存，并把数据返回到L3，L3缓存，并把数据返回到L2，L2缓存，并把数据
返回到L1，L1缓存，并把数据返回到CPU，CPU修改数据后返回到L1缓存，L1缓存对应的数据块被修改，根据写回机制，直到该块被覆盖时才会将修改返回到L2
但是L2可能已经没有对应的数据块了，此时就再重复上述过程，把对应的数据缓存到L2，然后修改L2，再根据写回机制，直到该块被覆盖时才会将修改返回到L3
当数据在L2时，可能CPU又需要修改对应的块，于是继续上边提到的步骤，只不过此时数据到L2就找到了，而不必到磁盘中找
这样就可以一波又一波的修改磁盘了 一波又一波！！想象一下

见 cacheLogic 函数 
*/

/*
test cases

./csim -s 1 -E 1 -b 1 -t traces/yi2.trace
9 8 6
./csim -s 4 -E 2 -b 4 -t traces/yi.trace
4 5 2
./csim -s 2 -E 1 -b 4 -t traces/dave.trace
2 3 1
./csim -s 2 -E 1 -b 3 -t traces/trans.trace
167 71 67 
./csim -s 2 -E 2 -b 3 -t traces/trans.trace
201 37 29
./csim -s 2 -E 4 -b 3 -t traces/trans.trace
212 26 10
./csim -s 5 -E 1 -b 5 -t traces/trans.trace
231 7 0
./csim -s 5 -E 1 -b 5 -t traces/long.trace
265189 21775 21743

完全正确
*/