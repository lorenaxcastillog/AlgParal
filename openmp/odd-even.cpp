#include <iostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono> 
using namespace std;

void odd_even(int *V, int n, int thread_count)
{
  int fase,i,tmp;
  for (fase = 0; fase < n; fase++) 
  {
   if (fase % 2 == 0)
   #pragma omp parallel for num_threads(thread_count) \
     default(none) shared(a, n) private(i, tmp)
   for (i = 1; i < n; i += 2) 
   {
 	if (V[i - 1] > V[i]) 
	{
  	  tmp = V[i - 1];
	  V[i - 1] = V[i];
	  V[i] = tmp;
	}
    }
    else
    #pragma omp parallel for num_threads(thread_count) \
      default(none) shared(a, n) private(i, tmp)
    for (i = 1; i < n - 1; i += 2)
    {
        if (V[i] > V[i+1]) 
	{
	  tmp = V[i+1];
	  V[i+1] = V[i];
	  V[i] = tmp;
	}
    }
   }
}

void odd_even_(int *V, int n, int thread_count)
{
   int fase,i,tmp;
   #pragma omp parallel num_threads(thread_count) \
	default(none) shared(a, n) private(i, tmp, fase)
   for (fase = 0; fase < n; fase++)
   {
     if (fase % 2 == 0)
     #pragma omp for
     for (i = 1; i < n; i += 2) 
     {
	if (V[i - 1] > V[i]) 
	{
	   tmp = V[i - 1];
	   V[i - 1] = V[i];
	   V[i] = tmp;
	}
     }
     else
     #pragma omp for
     for (i = 1; i < n - 1; i += 2) 
     {
	if (a[i] > a[i+1]) 
	{
	   tmp = a[i+1];
	   a[i+1] = a[i];
	   a[i] = tmp;
	}
     }
   }
}

void print(int *V,int n)
{
	for (int i = 0; i < n; ++i)
	{
	   cout<<V[i]<<" ";
	}
	cout<<endl;
}

int main(int argc, char const *argv[])
{	
	int thread_count=4;
	int n=20000;
	int *V=new int[n];
  	srand (time(NULL));
	for (int i = 0; i < n; ++i)
	{	
		int iSecret = rand() % n;
		V[i]=iSecret;
	}
	auto start = chrono::high_resolution_clock::now();
	odd_even(V,n,thread_count);
	
        auto finish = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed = finish - start;
	cout << "Tiempo Odd-Even: " << elapsed.count() <<endl; 
	
        auto start2 = chrono::high_resolution_clock::now();
	odd_even_(V,n,thread_count);
	
        auto finish2 = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed2 = finish2 - start2;
	cout << "Tiempo Odd-Even_2: " << elapsed2.count() <<endl; 
	return 0;
}
