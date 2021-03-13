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

addrType hexString2uint(char * str) //正的十六进制字符串转化到十进制unsigned int 
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
            sum += pow(16,size-(count++)) * (10+(currentchar-'a'));
        else if (currentchar>='0'&&currentchar<='9')
            sum += pow(16,size-(count++)) * (currentchar-'0');
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
    result.addr = hexString2uint(addr);

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

typedef struct cache //有2^s个组 每个组有E行
{
    bool valid; //有效位
    addrType mark; //标记

}cache;


int main(int argc,char * argv[]) //argv是储存char*的数组
{
    // printf("%d",hexString2uint("0x1Ab3"));
    // printf("%d",hexString2uint("1Ab3"));
    // printf("%d",decString2uint("13"));
    analyseInputData(argc,argv);
    printf("%d,%d,%d\r\n",E,s,b);
    
    char line[100];
    while (fgets(line,100,fp))
    {
       lineData thisd = analyseMemoryLine(line);
       printf("%d ",thisd.operatrion);
       printf("%ld \r\n",thisd.addr);
    }
    
    printSummary(0, 0, 0);
    return 0;
}
