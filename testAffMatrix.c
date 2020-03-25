#include "stdlib.h"
#include "stdio.h"

#define M_SIZE 4

void aff_matrix4x4(int M[M_SIZE][M_SIZE]);

int main(void)
{
    int M[4][4]={{4,0,0,2},{8,8,0,0},{32,0,2,0},{0,0,0,0}};
    aff_matrix4x4(M);
    return 0;
}

void aff_matrix4x4(int M[M_SIZE][M_SIZE])
{
    int i,j;
    for (j=3; j>=0;j--)
    {
        for(i=0; i<4; i++)
            printf("%d \t",M[i][j]);
        printf("\n");
    }
}
