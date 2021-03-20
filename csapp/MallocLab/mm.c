/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
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

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7) //上取整到ALIGNMENT的倍数


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define pack(size,used) (size|used) //将当前结点的总字节大小和是否被使用的标志位结合起来
                                    //当前结点的总字节大小是ALIGNMENT的倍数

#define headPtr(payload_ptr) ((char*)payload_ptr-SIZE_T_SIZE) 
#define get(ptr) (*(size_t*)ptr)
#define put(ptr,value) (*(size_t*)ptr=value)
#define getSize(payload_ptr) (get(headPtr(payload_ptr))& (~0x7)) //八的倍数 不受是否使用位的影响
#define footPtr(payload_ptr) ( (char*)payload_ptr + (getSize(payload_ptr)-2*SIZE_T_SIZE) )
#define setFlag(ptr,pos,flag) (put(ptr,get(ptr)& (~pos)| flag )) //对size_t 数据的pos位设置flag pos选中位，flag在对应的位设置0或1
// #define setUnused(payload_ptr) (put(headPtr(payload_ptr),pack(getSize(payload_ptr),0))&put(footPtr(payload_ptr),pack(getSize(payload_ptr),0)))
// #define setused(payload_ptr) (put(headPtr(payload_ptr),pack(getSize(payload_ptr),1)))

#define nextPtr(payload_ptr) ((char*)payload_ptr+getSize(payload_ptr))
#define prevPtr(payload_ptr) ((char*)payload_ptr - (get((char*)payload_ptr-2*SIZE_T_SIZE)&(~0x7)))
#define checkUsed(payload_ptr) (get(headPtr(payload_ptr))&0x1)

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    mem_init();
    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    int newsize = ALIGN(size + SIZE_T_SIZE);
    void *p = mem_sbrk(newsize);
    if (p == (void *)-1)
	return NULL;
    else {
        put(p,pack(newsize,0));
        return (void *)((char *)p + SIZE_T_SIZE);
    }
}

void coalesce(void * ptr) //合并块 这里就不进行合法性检查了 必须在合适的地方调用 也就是当前块必须是空闲块
{
    void * headptr = headPtr(ptr);
    size_t headvalue = get(headptr);
    int freeblock[2]; //前后的块是空闲块的话置1
    if (headvalue&0x2) // 说明上一个块是空闲块
        freeblock[0] = 1;
    else
        freeblock[0] = 0;
    if (checkUsed(nextPtr(ptr)))
        freeblock[1] = 0;
    else
        freeblock[1] = 1;
    
    if (freeblock[0]==1&&freeblock[1]==1) //前后都空闲
    {
        void * prevPtr = prevPtr(ptr);
        void * nextPtr = nextPtr(ptr);
        size_t allsize = getSize(prevPtr)+getSize(ptr)+getSize(nextPtr);
        void * prevheadPtr = headPtr(prevPtr);
        void * nextfootPtr = footPtr(nextPtr);
        put(prevheadPtr,pack(allsize,0));
        put(nextfootPtr,pack(allsize,0));
    }
    else if (freeblock[0]==1)
    {

    }
    else if (freeblock[1]==1)
    {

    }
    
}
/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    void * headptr = headPtr(ptr);
    setFlag(headptr,0x1,0);
    void * footptr = footPtr(ptr);
    setFlag(footptr,0x1,0);
    void * nextheadPtr = headPtr(nextPtr(ptr));
    setFlag(nextheadPtr,0x2,0x2); //下一个区块的倒数第二位代表上一个区块是否是空，如果为空，则置1 这个机制是为了减少内部碎片，当块分配了时，不需要foot
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












