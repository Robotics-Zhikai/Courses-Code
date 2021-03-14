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

