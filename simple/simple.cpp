#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <time.h>
#include <ctime>
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

//simple multiplication
void simpleMult(int **&A,int **&B, int **&C, int R_A, int C_A, int R_B, int C_B)
{
    if(C_A == R_B)
    {
        for(int i=0; i<R_A;i++)
        {
            for(int j=0; j<C_B;j++)
            {
                for(int k=0; k<C_A;k++)
                {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
    }
}

int main()
{
    int **A; int **B; int **C; int n = 2000;

    createNew(A,n,n);   createNew(B,n,n); createNew(C,n,n);
    fillMatrix(A,n,n);  fillMatrix(B,n,n);

    /*cout << "A"<<endl;
    printMatrix(A,n,n);
    cout << endl;

    cout << "B"<<endl;
    printMatrix(B,n,n);
    cout << endl;
*/
    clock_t start = clock();
    simpleMult(A,B,C,n,n,n,n);
    cout << "Tamaño matriz: " << n << " " << endl; 
    cout << "Tiempo de demora multiplicación simple: " << ((double)clock() - start) / CLOCKS_PER_SEC;
    cout << endl ;
  /*  cout << "C"<<endl;
    printMatrix(C,n,n);
    cout << endl;
 */
}



