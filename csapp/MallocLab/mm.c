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



 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

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


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define headSize (SIZE_T_SIZE)
#define footSize (SIZE_T_SIZE)
#define smallestBlockSize (ALIGN(headSize+footSize)) //最小的块大小就是这个值

#define prologueBlockSize (headSize+footSize) //序言块的大小
#define epilogueBlockSize (headSize+footSize) // 结尾块的大小
// #define prologueBlockContent ()
// #define epilogueBlockContent ()

//下边的这些define必须加上看起来多余的括号，否则会出现意想不到的错误
#define pack(size,used) ((size)|(used)) //将当前结点的总字节大小和是否被使用的标志位结合起来
                                    //当前结点的总字节大小是ALIGNMENT的倍数
#define head(payload_ptr) ((char*)(payload_ptr)-headSize) 
#define get(ptr) (*(size_t*)(ptr))
#define put(ptr,value) (*(size_t*)(ptr)=(value))
#define getSize(payload_ptr) (get(head(payload_ptr))& (~0x7)) //八的倍数 不受是否使用位的影响
#define foot(payload_ptr) ( (char*)(payload_ptr) + (getSize(payload_ptr)-headSize-footSize) )
#define setFlag(ptr,pos,flag) (put(ptr,(get(ptr)& (~pos))| flag )) //对size_t 数据的pos位设置flag pos选中位，flag在对应的位设置0或1
// #define setUnused(payload_ptr) (put(headPtr(payload_ptr),pack(getSize(payload_ptr),0))&put(footPtr(payload_ptr),pack(getSize(payload_ptr),0)))
// #define setused(payload_ptr) (put(headPtr(payload_ptr),pack(getSize(payload_ptr),1)))

#define nextPtr(payload_ptr) ((char*)(payload_ptr)+getSize(payload_ptr))
#define prevPtr(payload_ptr) ((char*)(payload_ptr) - (get((char*)(payload_ptr)-headSize-footSize)&(~0x7)))
#define checkUsed(payload_ptr) (get(head(payload_ptr))&0x1)
#define checkLastFree(ptr) (get(ptr)&0x2) //检查上一个块是否是空闲块


void fillinPrologueBlock(void * ptr)
{
    put(ptr,pack(0,1));
    ptr = (char *)ptr+headSize;
    put(ptr,pack(0,1));
}
void fillinEpilogueBlock(void * ptr)
{
    put(ptr,pack(0,1));
    ptr = (char *)ptr+headSize;
    put(ptr,pack(0,1));
}
void fillinNormalBlock(void * ptr,size_t size) //从ptr开始填充普通的空闲块,填充后的块大小至少为size
{
    size_t originsize = getSize((char*)ptr+headSize);

    // put(ptr,pack(originsize,1));
    // return;
    // if (originsize==0)
    //      printf("ptr:%x\r\n",ptr);
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
        put(ptr,pack(originsize-size,0));
        ptr = (char*)ptr + originsize - size - footSize;
        put(ptr,pack(originsize-size,0));
    }
}

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    mem_init();
    void *p = mem_sbrk(prologueBlockSize+epilogueBlockSize);
    if (p == (void *)-1)
	    return -1;
    fillinPrologueBlock(p); //序言块
    fillinEpilogueBlock((char*)p+prologueBlockSize); //尾块
    return 0;
}



void * findPlace(size_t size) //size是包括头和填充以及payload等的size
{
    char * p = mem_heap_lo(); //对mem_start_brk做了一个封装，不能直接调用该变量
    p = p + prologueBlockSize+headSize;
    size_t Cursize = 0;
    while((Cursize=getSize(p))!=0)
    {
        // printf("Cursize1:%d\r\n",Cursize);
        if(!checkUsed(p)&&Cursize>=size)
        {
            // printf("Cursize2:%d\r\n",Cursize);
            return head(p); //找到了适当的放置位置，将该位置返回
        }
            
        p=nextPtr(p);
    }

    return NULL; //说明没有找到适当的放置位置
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
        // printf("%x\r\n",mem_heap_hi());
        // printf("%x\r\n",loc);
        if (checkLastFree(loc))
        {
            loc = prevPtr((char*)loc+headSize);
            // printf("newsize:%d\r\n",newsize);
            // printf("fasf:%x\r\n",*(size_t*)loc);
            // printf("ss:%d\r\n",*(size_t*)((char*)loc-footSize));
            // printf("loc1:%x\r\n",loc);
            mem_sbrk(newsize - getSize(loc));
            loc = (char*)loc - headSize;
        }
        else
            mem_sbrk(newsize);
        put(loc,pack(newsize,0));
        fillinNormalBlock(loc,newsize);
        result = (char*)loc + headSize;
        loc = (char*)loc+newsize;
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

void coalesce(void * payloadPtr) //合并块 这里就不进行合法性检查了 必须在合适的地方调用 也就是当前块必须是空闲块
{
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
        // printf("1\r\n");
        allsize = getSize(prevptr)+getSize(payloadPtr)+getSize(nextptr);   
        put(prevheadPtr,pack(allsize,0));
        put(nextfootPtr,pack(allsize,0));
        // printf("%d\r\n",allsize);
    }
    else if (freeblock[0]==1) //前边空闲
    {

        // printf("2\r\n");
        
        // printf("%x\r\n",prevptr);
        // printf("%x\r\n",payloadPtr);
        // printf("%x\r\n",*(size_t *)((char*)payloadPtr-headSize-footSize));
        // printf("%d\r\n",getSize(payloadPtr));
        allsize = getSize(prevptr)+getSize(payloadPtr);
        put(prevheadPtr,pack(allsize,0));
        put(footptr,pack(allsize,0));
        // printf("%d\r\n",allsize);
    }
    else if (freeblock[1]==1) //后边空闲
    {
        // printf("3\r\n");
        allsize = getSize(payloadPtr)+getSize(nextptr);
        put(headptr,pack(allsize,0));
        put(nextfootPtr,pack(allsize,0));
        // printf("%d\r\n",allsize);
    }
    
}
/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    // printf("free:%d,%x\r\n",count++,ptr);
    // printf("getsize:%d\r\n",getSize(ptr));
    void * headptr = head(ptr);
    setFlag(headptr,0x1,0); //标记最后一位为0 说明释放了当前块
    void * footptr = foot(ptr);
    put(footptr,getSize(ptr)); //由于在头的倒数第二位设置了标记位 所以在非空闲块不需要脚，但是空闲块就需要脚 即便空闲块有脚也不影响内部碎片
    // printf("footptr:%x\r\n",footptr);
    // printf("footptrvalue:%d\r\n",get(footptr));
    void * nextheadPtr = head(nextPtr(ptr));
    // printf("%x\r\n",*(size_t*)nextheadPtr);
    setFlag(nextheadPtr,0x2,0x2); //下一个区块的头的倒数第二位代表上一个区块是否是空，如果为空，则置1 这个机制是为了减少内部碎片，当块分配了时，不需要foot

    coalesce(ptr);//加了这个合并的话由57分增到了62分
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    return realloc(ptr,size);
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
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












