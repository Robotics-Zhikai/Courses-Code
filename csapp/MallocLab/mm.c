/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
version 1.0
简单的隐式线性链表实现，分数是52（util）+9（thru）= 62/100，不包括realloc
#define DEFAULT_TRACEFILES \
  "amptjp-bal.rep",\
  "cccp-bal.rep",\
  "cp-decl-bal.rep",\
  "expr-bal.rep",\
  "coalescing-bal.rep",\
  "random-bal.rep",\
  "random2-bal.rep",\
  "binary-bal.rep",\
  "binary2-bal.rep"
非空闲块时没有foot，空闲块时有foot，充分利用头的低3位数据标记前一个相邻块的空闲与否
立即合并 首次适配 




version 2.0 
51（util）+28（thru）= 79/100（不包括realloc测试集）；
1、每一个空闲块的head包括implicithead和explicithead和foot（只包括当前块的大小）；
2、explicithead由firstaddr和secondaddr组成；
3、分配了的非空闲块的head仍然包括implicithead和explicithead，没有foot
这里边implicithead为8字节 explicithead为16字节；
4、分离表以ceil(log2(size))作为分组方式；
5、子类的头结点由firstaddr链接的单向链表链接起来，子类的头结点的secondaddr指向本子类的第一个内容结点；
6、本子类的内容结点由一个双向链表连接起来，firtaddr储存指向下一个内容节点，secondaddr指向上一个内容结点；
7、implicithead部分的除低三位部分存放本块的大小，最低一位存放本块是否是空闲块，次低位存放本块相邻的前一块是否是空闲块
倒数低三位存放本块是否是子类的头结点；
8、每次新创建的空闲块以线性时间找到子类，并在该子类以常数时间加入到对应的子类中；
9、malloc一个size后，如果首次适配的空闲块的大小大于smallestBlockSize，那么就把剩下的块加入到空闲块分离表中，
否则直接把整个空闲块都作为malloc空间；
10、free了一块空间后，会立即合并相邻的空闲块（如果存在的话）；

利用上述策略除realloc测试集都过了，但是realloc测试集由于内存耗尽的问题过不了。
因此，需要想一个优化策略。




version 3.0
1、首先，MAX_HEAP是20*(1<<20)B,也就是说表示一个块的字节大小绝对不会超过32位（4字节）
因此implicitheadSize可以降到4B，同样的道理，footSize也可以降到4B
然后，指针也是四字节，因此explicitheadSize为2*4B；
2、然后对于非空闲块，只要有一个implicithead就够了 空闲块的话最小为implicithead+explicithead+foot（4+8+4=16B）
但是，算最小块大小的话，只能按空闲块去算，不能按非空闲块算；
但是，如果把非空闲块和空闲块的头部区分开来的话，又得写很多代码，容易出错，时间有限就不写了。经过分析，这样也不会造成利用率的明显降低
3、空闲块的头指针必须是4的倍数且不能是8的倍数，这样才能保证malloc的指针是8的倍数，
要想实现这样的结构，只需要在最开始垫一个四字节的offsetblock就行。

4、3.0实现至此，分数为52 (util) + 40 (thru) = 92/100 （不含realloc测试集）
利用率稍有增加，吞吐率大幅度增加。主要原因是之前都是把地址看成8个字节的，现在看成了四个字节，
可能在汇编级每条指令执行的CPU周期更少，进而造成大的吞吐量。
由此得到一条经验：在编程时尽量提高数据的利用率，减少数据的冗余，否则可能会降低效率

5、而且在realloc的时候，不能简单直接的分配r所指定的大小内存，而应该充分利用已有的内存大小，将已有的内存利用起来，
否则如果r的大小非常大的话，很容易就超出存储空间了
将最后一个块如果是空闲块也利用上，那么就解决了realloc超出存储空间的问题，最终整个测试集上得到的分数是
46 (util) + 39 (thru) = 85/100(含realloc测试集)




version 4.0（时间有限，暂未实现 有前三个版本就差不多了 之后学学malloc的源码是怎么写的 如果时间充足再写吧）
1、目前还可以想到的优化是子类的分组方式可以改一改，现在的子类分组是通过ceil(log2(size))分的，
这就造成了子类的容纳空闲块大小范围分布不均匀，进而造成某些子类的内容结点过多；
但是如果要均匀分配的话，又会造成子类头结点过多，分配时查找花销大的问题。且具体均匀分的间隔是多少没办法确定；

2、还可以的一种方法是每次新加入子类的头结点时,根据一定范围的值维护一个最大堆，堆的每个结点就是各子类的头结点，
然后每个子类的内容结点又能以该子类头结点为根又维护一个最大堆
这样可以把空闲块插入的时间降到logn，malloc的时间降到logn，而又不改变空间利用率，增大了吞吐量；

3、再没有想到更好的方法，需要阅读malloc、realloc的源码才能知道。

*/
/*
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "HIT",
    /* First member's full name */
    "Zhikai",
    /* First member's email address */
    "@stu.hit.edu.cn",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};
int count = 0;
/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7) //上取整到ALIGNMENT的倍数


//#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))
#define SIZE_T_SIZE 4

#define implicitheadSize SIZE_T_SIZE //存放当前块的大小
typedef char* addrType; //地址的类型 64位系统就是unsighed long的sizeof
#define addrSize SIZE_T_SIZE
#define explicitheadSize (2*addrSize) //存放连接前后链表的地址 先是next 然后是last

#define headSize (implicitheadSize+explicitheadSize)
#define footSize (SIZE_T_SIZE)
#define smallestBlockSize (ALIGN(headSize+footSize)) //最小的块大小就是这个值

#define prologueBlockSize (headSize+footSize) //序言块的大小
#define epilogueBlockSize (headSize+footSize) // 结尾块的大小
#define offsetBlockSize SIZE_T_SIZE //为了保证8字节对齐 需要在一开始加上这么大的offsetblock
// #define prologueBlockContent ()
// #define epilogueBlockContent ()

//下边的这些define必须加上看起来多余的括号，否则会出现意想不到的错误
#define pack(size,used) ((size)|(used)) //将当前结点的总字节大小和是否被使用的标志位结合起来
                                    //当前结点的总字节大小是ALIGNMENT的倍数
#define head(payload_ptr) ((char*)(payload_ptr)-headSize) 
#define get(ptr) (*(unsigned int*)(ptr))
#define put(ptr,value) (*(unsigned int*)(ptr)=(value))
#define getSize(payload_ptr) (get(head(payload_ptr))& (~0x7)) //八的倍数 不受是否使用位的影响
#define foot(payload_ptr) ( (char*)(payload_ptr) + (getSize(payload_ptr)-headSize-footSize) )
#define setFlag(ptr,pos,flag) (put(ptr,(get(ptr)& (~pos))| flag )) //对size_t 数据的pos位设置flag pos选中位，flag在对应的位设置0或1
// #define setUnused(payload_ptr) (put(headPtr(payload_ptr),pack(getSize(payload_ptr),0))&put(footPtr(payload_ptr),pack(getSize(payload_ptr),0)))
// #define setused(payload_ptr) (put(headPtr(payload_ptr),pack(getSize(payload_ptr),1)))

#define nextPtr(payload_ptr) ((char*)(payload_ptr)+getSize(payload_ptr))
#define prevPtr(payload_ptr) ((char*)(payload_ptr) - (get((char*)(payload_ptr)-headSize-footSize)&(~0x7)))

#define checkUsed(payload_ptr) (get(head(payload_ptr))&0x1)
#define checkLastFree(headptr) (get(headptr)&0x2) //检查上一个块是否是空闲块
#define checksubclassHeadNode(headptr) (get(headptr)&0x4) //检查本块是否是子类的头结点

#define firstAddr(payload_ptr) ((char*)head(payload_ptr)+implicitheadSize)
#define secondAddr(payload_ptr) (firstAddr(payload_ptr)+addrSize)
#define getfirstAddr(payload_ptr) (get(firstAddr(payload_ptr))) //得到第一个地址，第一个地址的含义因具体应用的不同而不同
#define getsecondAddr(payload_ptr) (get(secondAddr(payload_ptr))) //得到第二个地址，同样，第二个地址的含义因具体应用的不同而不同

int calculateGroup(size_t size) //计算size大小的块在哪个group中
{
    // printf("%f\r\n",ceil(log2(size)));
    if (size<=0)
        return -1; //这是异常情况
    return ceil(log2(size)); //一般情况下返回值肯定大于0 
}

void addtoSegregateList(void * freePayloadptr) //将空闲块加入到segregateList中
//调用这个函数时，freePayloadptr所代表的块必须有head和foot
//这个只改变显示链表指针的值 不改变其他任何东西
//每个子类的头结点中，firstAddr指向的是下一个子类的头结点；而secondAddr指向的不是上一个子类的头结点，而是指向的是本子类的下一个内容结点
//每个子类的内容结点的firstAddr指向的是本子类的下一个内容结点，secondAddr指向的是本子类的上一个内容结点
//指向的地址都是payload的地址
{
    setFlag(head(freePayloadptr),0x4,0); //这个就是个保险措施，以防外边的代码出现忘了置0的情况 如果外边代码没问题，这个完全可以去掉

    size_t size = getSize(freePayloadptr);
    int group = calculateGroup(size);

    char * p = mem_heap_lo()+offsetBlockSize+headSize;
    char * curpayloadPtr = p;
    while((curpayloadPtr = getfirstAddr(curpayloadPtr))!=NULL)
    {
        if(calculateGroup(getSize(curpayloadPtr))==group) //说明找到对应的group了,要把这个空闲块添加进这个group中去
        {
            
            addrType storeAddr = getsecondAddr(curpayloadPtr);
            if (storeAddr==NULL) //说明找到的本子类中只有一个头结点，也就是本子类只有一个空闲块，那么将新的块添加进去
            {
                //printf("1\r\n");
                put(firstAddr(freePayloadptr),NULL); //新的内容结点的firstAddr指向NULL
                put(secondAddr(freePayloadptr),curpayloadPtr); //新的内容结点的secondAddr指向curpayloadPtr
                put(secondAddr(curpayloadPtr),freePayloadptr); //头结点的secondAddr指向新加入的内容结点
            }
            else //说明本子类有内容结点 把新块加在头部 LIFO
            {
                //printf("2\r\n");
                // addrType storeAddrnext = getfirstAddr(storeAddr);
                put(firstAddr(freePayloadptr), storeAddr);
                put(secondAddr(curpayloadPtr),freePayloadptr); //后向列表

                put(secondAddr(storeAddr),freePayloadptr);
                put(secondAddr(freePayloadptr),curpayloadPtr); //前向列表
            }
            return;
        }
        p = curpayloadPtr;
    }
    //到这里时说明没有找到对应的group，此时需要新建一个group 这是子类的头结点 不是子类的内容结点 子类的头结点跟其他子类的头结点连在一起
    put(firstAddr(p),freePayloadptr);
    put(firstAddr(freePayloadptr),NULL);
    put(secondAddr(freePayloadptr),NULL);
    setFlag(head(freePayloadptr),0x4,0x4); //倒数第三位标记是否是子类的头结点 如果是的话就置1 否则置0
}

void removefromSegregateList(void * freePayloadptr) //将空闲块从segregateList中删除 空闲块的地址必须正确 否则未定义行为
//这个只改变显示链表指针的值 不改变其他任何东西
{
    if (checksubclassHeadNode(head(freePayloadptr))) //说明是子类的头结点
    {
        //由于子类的头结点构成的链表不是一个双向链表，因此对其delete需要线性复杂度的时间 但是子类头结点的数量不多，完全是可以接受的复杂度
        char * p = mem_heap_lo()+offsetBlockSize+headSize;
        char * curpayloadPtr = p;
        while((curpayloadPtr = getfirstAddr(curpayloadPtr))!= freePayloadptr){p=curpayloadPtr;} 
        //当while退出时就可以得到freePayloadptr的前一个子类头结点 储存在p中

        addrType addrstore = getsecondAddr(freePayloadptr);
        if (addrstore==NULL) //说明该子类就一个头结点
            put(firstAddr(p),getfirstAddr(freePayloadptr)); //直接把该块删除即可
        else
        {
            //否则需要把内容结点转化为子类的头结点 addrstore是新的转化为子类头结点的结点
            put(firstAddr(p),addrstore);
            put(secondAddr(addrstore),getfirstAddr(addrstore));
            put(firstAddr(addrstore),getfirstAddr(freePayloadptr));

            addrType thisStore = getsecondAddr(addrstore);
            if (thisStore!=NULL) //如果等于NULL的话就什么也不做 否则就把后一个的前向结点指向新的结点
                put(secondAddr(thisStore),addrstore); //本来链表的前向指针指向的是待删除的loadptr，但是这时需要指向addrstore
            setFlag(head(addrstore),0x4,0x4); //标记为子类的头结点
        }
    }
    else
    {
        addrType lastAddr = getsecondAddr(freePayloadptr);
        addrType nextAddr = getfirstAddr(freePayloadptr);
        if (checksubclassHeadNode(head(lastAddr))) //如果上一个结点是子类的头结点，那么由于结构的不同，释放时的情况会有所变化
        {
            put(secondAddr(lastAddr),nextAddr);
            if(nextAddr!=NULL)
                put(secondAddr(nextAddr),lastAddr);
        }
        else
        {
            put(firstAddr(lastAddr),nextAddr);
            if (nextAddr!=NULL)
                put(secondAddr(nextAddr),lastAddr);
        }
    }
}

void fillinPrologueBlock(void * ptr)
{
    put(ptr,pack(0,1)); //隐式的值为0
    ptr = (char*)ptr + implicitheadSize;
    put(ptr,NULL); //设置显示的链表指针初始为0
    ptr = (char*)ptr + addrSize;
    put(ptr,NULL);
    ptr = (char *)ptr+addrSize;
    put(ptr,pack(0,1));
}
void fillinEpilogueBlock(void * ptr)
{
    put(ptr,pack(0,1)); //隐式的值为0
    ptr = (char*)ptr + implicitheadSize;
    put(ptr,NULL); //设置显示的链表指针初始为0
    ptr = (char*)ptr + addrSize;
    put(ptr,NULL);
    ptr = (char *)ptr+addrSize;
    put(ptr,pack(0,1));
}
void fillinNormalBlock(void * ptr,size_t size) //从ptr开始填充普通的空闲块,填充后的块大小至少为size
{ 
    addrType payloadPtr = (char*)ptr+headSize;
    size_t originsize = getSize(payloadPtr);

    // put(ptr,pack(originsize,1));
    // return;
    // if (originsize==0)
    //      printf("ptr:%x\r\n",ptr);

    removefromSegregateList(payloadPtr);
    if (originsize-size < smallestBlockSize)
    {
        put(ptr,pack(originsize,1));
        setFlag((char*)ptr+originsize,0x2,0); //后边跟着的块肯定不能指示其前面的块是空闲的
    }
    else
    {
        put(ptr,pack(size,1));
        ptr = (char*)ptr + size;
        // printf("originsize:%u\r\n",originsize);
        //  printf("size:%u\r\n",size);

        addrType newptr = ptr+headSize;
        put(ptr,pack(originsize-size,0));
        ptr = (char*)ptr + originsize - size - footSize;
        put(ptr,pack(originsize-size,0));
        addtoSegregateList(newptr); //将分割出来的新的空闲块加入到分离表中
    }
}

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    mem_init();
    void *p = mem_sbrk(prologueBlockSize+epilogueBlockSize+offsetBlockSize);
    if (p == (void *)-1)
	    return -1;

    put(p,0); //offsetblock 为了保证malloc的地址是8的倍数
    p = (char*)p+offsetBlockSize;
    fillinPrologueBlock(p); //序言块
    fillinEpilogueBlock((char*)p+prologueBlockSize); //尾块
    return 0;
}

void * findPlace(size_t size) //size是包括头和填充以及payload等的size
{

    int group = calculateGroup(size);
    char * p = mem_heap_lo()+offsetBlockSize+headSize;
    char * curpayloadPtr = p;

    while((curpayloadPtr=getfirstAddr(curpayloadPtr))!=NULL)
    {
        if (calculateGroup(getSize(curpayloadPtr))>=group)
        {
            char * nextContentNode = getsecondAddr(curpayloadPtr); 
            //第一个碰到的节点是子类的头结点，子类的头结点和子类的内容节点结构还不是一样的
            if (getSize(curpayloadPtr)>=size)
                return head(curpayloadPtr); //说明找到了，就是子类的头结点
            else
            {
                while (nextContentNode!=NULL)
                {
                    if (getSize(nextContentNode)>=size)
                        return head(nextContentNode);
                    nextContentNode = getfirstAddr(nextContentNode); //第二个碰到的节点是子类的第一个内容节点
                }
            }
        }
    }
    //说明没找到
    //返回NULL
    return NULL;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    size_t newsize = ALIGN(size + headSize); 
    // printf("newsize:%d\r\n",newsize);
    void * loc = findPlace(newsize);
    void * result;
    // printf("loc:%x\r\n",loc);
    if (loc == NULL) //说明没找到合适的能存放的地方
    {
        loc = (char*)mem_heap_hi()+1-epilogueBlockSize;
        //printf("%d\r\n",checkLastFree(loc)); //然后发现很多时候上一个块都是free掉的

        void * retptr;
        size_t allocsize;
        if (checkLastFree(loc))
        {
            loc = prevPtr((char*)loc+headSize);
            allocsize = newsize - getSize(loc);
            retptr = mem_sbrk(allocsize);
            removefromSegregateList(loc);
            loc = (char*)loc - headSize;
        }
        else
            retptr = mem_sbrk(newsize);// 直接分配newsize这么大的内存
        if (retptr==(void*)-1)
            return NULL; //说明没地方了 分配完了

        put(loc,pack(newsize,1)); //直接在新分配的内存中分配就行
        result = (char*)loc+headSize;
        loc = (char*)loc + newsize;
        fillinEpilogueBlock(loc);
    }
    else
    {
        fillinNormalBlock(loc,newsize);
        result = (char*)loc + headSize;
    }
   // printf("malloc:%d,%x\r\n",count++,result);
    return result;
}

void * coalesce(void * payloadPtr) //合并块 这里就不进行合法性检查了 必须在合适的地方调用 也就是当前块必须是空闲块
{
    void * result = payloadPtr;

    void * headptr = head(payloadPtr);
    size_t headvalue = get(headptr);
    int freeblock[2]; //前后的块是空闲块的话置1
    if (headvalue&0x2) // 说明上一个块是空闲块
        freeblock[0] = 1;
    else
        freeblock[0] = 0;
    if (checkUsed(nextPtr(payloadPtr)))
        freeblock[1] = 0;
    else //说明下一个是空闲块
        freeblock[1] = 1;
    
    void * prevptr = prevPtr(payloadPtr);
    void * nextptr = nextPtr(payloadPtr);
    void * prevheadPtr = head(prevptr);
    void * nextfootPtr = foot(nextptr);
    void * footptr = foot(payloadPtr);
    size_t allsize = 0;
    
    if (freeblock[0]==1&&freeblock[1]==1) //前后都空闲
    {
        removefromSegregateList(prevptr);
        removefromSegregateList(nextptr);

        allsize = getSize(prevptr)+getSize(payloadPtr)+getSize(nextptr);   
        put(prevheadPtr,pack(allsize,0));
        put(nextfootPtr,pack(allsize,0));
        result = prevptr;
    }
    else if (freeblock[0]==1) //前边空闲
    {
        removefromSegregateList(prevptr);

        allsize = getSize(prevptr)+getSize(payloadPtr);
        put(prevheadPtr,pack(allsize,0));
        put(footptr,pack(allsize,0));
        // printf("%d\r\n",allsize);
        result = prevptr;
    }
    else if (freeblock[1]==1) //后边空闲
    {
        removefromSegregateList(nextptr);

        allsize = getSize(payloadPtr)+getSize(nextptr);
        put(headptr,pack(allsize,0));
        put(nextfootPtr,pack(allsize,0));
        result = payloadPtr;
    }
    addtoSegregateList(result);
    return result; //返回合并后的空闲块的payload地址
}


/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    //printf("free:%d,%x\r\n",count++,ptr);
    // printf("getsize:%d\r\n",getSize(ptr));
    void * headptr = head(ptr);
    setFlag(headptr,0x1,0); //标记最后一位为0 说明释放了当前块
    setFlag(headptr,0x4,0); //表明当前释放的空闲块默认不是子类的头结点
    void * footptr = foot(ptr);
    put(footptr,get(headptr)); //由于在头的倒数第二位设置了标记位 所以在非空闲块不需要脚，但是空闲块就需要脚 即便空闲块有脚也不影响内部碎片
    void * nextheadPtr = head(nextPtr(ptr));
    setFlag(nextheadPtr,0x2,0x2); //下一个区块的头的倒数第二位代表上一个区块是否是空，如果为空，则置1 这个机制是为了减少内部碎片，当块分配了时，不需要foot

    void * coalescePtr = coalesce(ptr);//加了这个合并的话由57分增到了62分；变成显式链表 分离表之后涨到了79
    
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    // copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    copySize = getSize(oldptr);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}

int mm_check() //检查堆一致性 用来帮助debug的，在提交程序中不应包括这个函数，会降低吞吐量
{
    return 0;
}












