#include <iostream>
#include <cuda.h>
using namespace std;

__global__ void MultMatrizVectKernel(float *A, float *B, float *C, int n)
{
  int i = n * blockIdx.x; 
  if(i < n*n)
  {
    for(int j = 0; j < n ; ++j)
    {
      C[blockIdx.x] += A[i + j] * B[j];
    }
  }
} 

void MultMatrizVector(float *A, float *B, float *C, int n)
{
  float *d_A, *d_B, *d_C; 
  size_t size_A = n*n * sizeof(float);
  size_t size_B = n * sizeof(float);
  size_t size_C = n * sizeof(float);

  cudaMalloc((void **) &d_A, size_A);
  cudaMalloc((void **) &d_B, size_B);
  cudaMalloc((void **) &d_C, size_C);

  cudaMemcpy(d_A, A, size_A, cudaMemcpyHostToDevice);
  cudaMemcpy(d_B, B, size_B, cudaMemcpyHostToDevice);

  MultMatrizVectKernel<<< n, 1 >>>(d_A, d_B, d_C, n);

  cudaMemcpy(C, d_C, size_C, cudaMemcpyDeviceToHost);

  cudaFree(d_A); cudaFree(d_B); cudaFree(d_C);
}

int main(void)
{
    float *A, *B , *C; 
    int n = 10 ; 
    A = new float[n*n];
    B = new float[n];
    C = new float[n];
    for (int i = 0; i < n*n; ++i)
    {
        A[i] = i;
    }

    for (int i = 0; i < n; ++i)
    {
        B[i] = i;
    }
    
    MultMatrizVector(A, B, C ,n); 
 
    for(int i = 0; i < n ; ++i)
    {
       cout << C[i] << " ";

    }
  return 0;
}
