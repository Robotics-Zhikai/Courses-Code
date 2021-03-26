/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i, j,k_x,k_y;
    int a0,a1,a2,a3,a4,a5,a6,a7;
    if (M==N && M==32)
    {
        for (k_x = 0;k_x<=24;k_x=k_x+8)
        {
            for (k_y = 0;k_y<=24;k_y=k_y+8)
            {
                if (k_x==k_y)
                {
                    a0 = A[k_x+0][k_x+0];a1 = A[k_x+1][k_x+1];a2 = A[k_x+2][k_x+2];a3 = A[k_x+3][k_x+3];
                    a4 = A[k_x+4][k_x+4];a5 = A[k_x+5][k_x+5];a6 = A[k_x+6][k_x+6];a7 = A[k_x+7][k_x+7];
                    //用多出来的八个局部变量，主要是为了避免对于B[i][i] = A[i][i]对角线上的多出来的两个miss
                    //注意在写B时由于有写回写分配机制，也需要将B对应的块加载过来
                    //在此题中所示的缓存器结构以及32*32的数组的情况下 对角线上的元素A和B在缓存器上是重叠的
                    for(i=0+k_y;i<8+k_y;i++)
                    {
                        for(j=0+k_x;j<8+k_x;j++)
                        {
                            if (i!=j)
                            {
                                B[i][j] = A[j][i];
                            }
                            else
                            {
                                if(i-k_x==0)
                                    B[i][j] = a0;
                                else if (i-k_x==1)
                                    B[i][j] = a1;
                                else if (i-k_x==2)
                                    B[i][j] = a2;
                                else if (i-k_x==3)
                                    B[i][j] = a3;
                                else if (i-k_x==4)
                                    B[i][j] = a4;
                                else if (i-k_x==5)
                                    B[i][j] = a5;
                                else if (i-k_x==6)
                                    B[i][j] = a6;
                                else if (i-k_x==7)
                                    B[i][j] = a7;
                            }
                        }
                    }   
                }
                else
                {
                    for(i=0+k_y;i<8+k_y;i++)
                    {
                        for(j=0+k_x;j<8+k_x;j++)
                        {
                            B[i][j] = A[j][i];
                        }
                    }
                }
            }
        }
    }
    else if (M==N&&M==64) //这个没有到满分 现在时间比较紧，以后有时间再看吧
    {
        for (k_x=0;k_x<=60;k_x=k_x+4)
        {
            for(k_y=0;k_y<=60;k_y=k_y+4)
            {
                if (k_x==k_y)
                {
                    a0 = A[k_x+0][k_x+0];a1 = A[k_x+1][k_x+1];a2 = A[k_x+2][k_x+2];a3 = A[k_x+3][k_x+3];
                    for(i=k_x;i<k_x+4;i++)
                    {
                        for(j=k_y;j<k_y+4;j++)
                        {
                            if (i!=j)
                                B[i][j] = A[j][i];
                            else
                            {
                                if (i-k_x==0)
                                    B[i][i]=a0;
                                else if (i-k_x==1)
                                    B[i][i]=a1;
                                else if (i-k_x==2)
                                    B[i][i]=a2;
                                else if (i-k_x==3)
                                    B[i][i]=a3;
                            }
                        }
                    }
                }
                else
                {
                    for(i=k_x;i<k_x+4;i++)
                    {
                        for(j=k_y;j<k_y+4;j++)
                        {
                            B[i][j] = A[j][i];
                        }
                    }
                }
                
            }
        }
        /*
            //满分答案
        作者：马天猫Masn
        链接：https://zhuanlan.zhihu.com/p/28585726
        来源：知乎
        著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
        for(i=0;i<64;i+=8) //按照他这个代码把数字画出来就能理解了 语言描述对机理描写很不准 按照程序写一写就知道了
        {
            for(j=0;j<64;j+=8)
            {
                for(k=j;k<j+4;++k) //这个是上边的4*8的块
                {
                    a1=A[k][i];a2=A[k][i+1];a3=A[k][i+2];a4=A[k][i+3];
                    a5=A[k][i+4];a6=A[k][i+5];a7=A[k][i+6];a8=A[k][i+7];

                    B[i][k]=a1;B[i][k+4]=a5;B[i+1][k]=a2;B[i+1][k+4]=a6;
                    B[i+2][k]=a3;B[i+2][k+4]=a7;B[i+3][k]=a4;B[i+3][k+4]=a8;  
                    //分成4*4的小块是好理解的，然后为了充分利用每次命中时移动的块，把另外的4*4的小块也用起来，当成中间矩阵                        
                }
                for(k=i;k<i+4;++k) //然后通过引用中间矩阵进行矩阵的转置
                {
                    a1=B[k][j+4];a2=B[k][j+5];a3=B[k][j+6];a4=B[k][j+7];
                    a5=A[j+4][k];a6=A[j+5][k];a7=A[j+6][k];a8=A[j+7][k];

                    B[k][j+4]=a5;B[k][j+5]=a6;B[k][j+6]=a7;B[k][j+7]=a8;
                    B[k+4][j]=a1;B[k+4][j+1]=a2;B[k+4][j+2]=a3;B[k+4][j+3]=a4;
                }
                for(k=i+4;k<i+8;++k) //最后处理右下角的矩阵转置
                {
                    a1=A[j+4][k];a2=A[j+5][k];a3=A[j+6][k];a4=A[j+7][k];

                    B[k][j+4]=a1;B[k][j+5]=a2;B[k][j+6]=a3;B[k][j+7]=a4;
                }
            }
        }



        */

    }
    else 
    {
        a0 = 16;
        for(k_y=0;k_y<N;k_y=k_y+a0)
        {
            for(k_x=0;k_x<M;k_x=k_x+a0)
            {
                for(j=k_x;j<((k_x+a0)>=M?M:(k_x+a0));j++)
                {
                    for(i=k_y;i<((k_y+a0>=N)?N:(k_y+a0));i++)
                    {
                        B[j][i] = A[i][j];
                    }
                }
            }
        }
    }
    //分块技术很重要 可以充分利用缓存 减小miss 
    //当缓存空间充分大时，正常的读取操作就可以应付，因为没有随着地址增长大地址取%重复映射到小地址的情况
    //不同的访问顺序能造成不同的缓存命中率
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

