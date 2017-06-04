#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <time.h>
using namespace std;

//create matrix R rows and C columns
void createNew(int **&A, int R, int C)
{
    A = new int *[R];
    for(int i=0;i < R; ++i)
    {
        A[i] = new int[C];
    }
}

//fill random matrix
void fillMatrix(int **&A, int R, int C)
{
    for(int i=0; i < R; i++)
    {
        for(int j=0; j < C; j++)
        {
            A[i][j] = rand()%10;
        }
    }
}

//print matrix
void printMatrix(int **&A, int R_A, int C_A)
{
    for(int i=0; i < R_A;i++)
    {
        for(int j=0; j < C_A;j++)
        {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }
}

//block multiplication
void blockMult(int **&A,int **&B, int **&C, int sizeMatrix, int sizeBlock)
{
    for(int i=0; i<sizeMatrix; i+=sizeBlock)
    {
        for(int j=0; j<sizeMatrix; j+=sizeBlock)
        {
            for(int k=0; k<sizeMatrix; k+=sizeBlock)
            {
                for(int i_=i; i_<i+sizeBlock; ++i_)
                {
                    for(int j_=j; j_<j+sizeBlock; ++j_)
                    {
                        for(int k_=k; k_<k+sizeBlock; ++k_)
                        {
                            C[i_][j_]+=A[i_][k_]*B[k_][j_];
                        }
                    }
                }
            }
        }
    }
}


int main()
{
    int **A; int **B; int **C; int **D; int n = 2000, b = 200 ;

    createNew(A,n,n);   createNew(B,n,n); createNew(C,n,n); createNew(D,n,n);
    fillMatrix(A,n,n);  fillMatrix(B,n,n);
/*
    cout << "A"<<endl;
    printMatrix(A,n,n);
    cout << endl;

    cout << "B"<<endl;
    printMatrix(B,n,n);
    cout << endl;
*/
    clock_t start = clock();
    blockMult(A,B,C,n,b);
    cout << "Tamaño matriz: " << n << " " << endl; 
    cout << "Tiempo de demora multiplicación bloques: " << ((double)clock() - start) / CLOCKS_PER_SEC;
    cout << endl ;
/*
    cout << "C"<<endl;
    printMatrix(C,n,n);
    cout << endl;

*/

}
