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
void transpose_submit(int M, int N, int A[N][M], int B[M][N]) {
    if ((N==32) && (M==32)) {
        int i,j,i1,j1,temp;
        for (i=0; i<4; i++) {
            for (j=0; j<4; j++) {
                for (i1=0; i1<8; i1++) {
                    for (j1=0; j1<8; j1++) {
                        if (i1!=j1) {
                            B[j1+j*8][i1+i*8]=A[i1+i*8][j1+j*8];
                        }
                    }
                    if (i1!=0) {
                        B[i1-1+j*8][i1-1+i*8]=temp;
                    }
                    temp=A[i1+i*8][i1+j*8];
                }
                B[j*8+7][i*8+7]=temp;
            }
        }
    } else if ((N==64)&&(M==32)) {
        int i,j,i1,j1;
        for (i=0; i<4; i++) {
            for (j=0; j<8; j++) {
                for (i1=0; i1<8; i1++) {
                    for (j1=0; j1<8; j1++) {
                        B[i*8+i1][j*8+j1]=A[j*8+j1][i*8+i1];
                    }
                }
            }
        }
    } else if ((N==64)&&(M==64)) {
    	int i,j,k,t0,t1,t2,t3,t4,t5,t6,t7;
        for (j=0; j<8; j++) {
            for (i=0; i<8; i++) {
                for (k=0; k<8; k++) {
                    t0=A[i*8+k][j*8];
                    t1=A[i*8+k][j*8+1];
                    t2=A[i*8+k][j*8+2];
                    t3=A[i*8+k][j*8+3];
                    if (k==0) {
                        t4=A[i*8+k][j*8+4];
                        t5=A[i*8+k][j*8+5];
                        t6=A[i*8+k][j*8+6];
                        t7=A[i*8+k][j*8+7];
                    }
                    
                    B[j*8][i*8+k]=t0;
                    B[j*8+1][i*8+k]=t1;
                    B[j*8+2][i*8+k]=t2;
                    B[j*8+3][i*8+k]=t3;
                }
                for (k=7; k>0; k--) {
                    t0=A[i*8+k][j*8+4];
                    t1=A[i*8+k][j*8+5];
                    t2=A[i*8+k][j*8+6];
                    t3=A[i*8+k][j*8+7];
                    B[j*8+4][i*8+k]=t0;
                    B[j*8+5][i*8+k]=t1;
                    B[j*8+6][i*8+k]=t2;
                    B[j*8+7][i*8+k]=t3;
                }
                B[j*8+4][i*8]=t4;
                B[j*8+5][i*8]=t5;
                B[j*8+6][i*8]=t6;
                B[j*8+7][i*8]=t7;
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

