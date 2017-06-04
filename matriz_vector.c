#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <sys/time.h>

int NUM_OF_THREADS, M, N;
float* A;
float* x;
float* y;
float* y_s;
pthread_mutex_t mutex;
 

long Tiempo_segundos(timeval inicio, timeval fin)
{
   long seg, useg;
   seg  = fin.tv_sec  - inicio.tv_sec;
   useg = fin.tv_usec - inicio.tv_usec;
   return ((seg) * 1000 + useg/1000.0) + 0.5;
}

void Llenar_matriz(float A[], int f, int c)
{
   int i, j;
   for (i = 0; i < f; i++)
   {
      for (j = 0; j < c; j++)
      {
         A[i * c + j] = (rand() % 100); //
      }
   }
}

void Llenar_vector(float x[], int f)
{
   int i;
   for (i = 0; i < f; i++)
   {
      x[i] = (rand() % 100); //
   }
}

void Print_matriz(float A[], int f, int c) 
{
   int i, j;
   for (i = 0; i < f; i++) 
   {
      for (j = 0; j < c ; j++)
      {
	   printf("%f  ", A[i*c + j]);
      }
      printf("\n");
   }
}

void Print_vector(float x[], int f) 
{
   int i;
   for (i = 0; i < f; i++)
   {
      printf("%f  ", x[i]);
   }
   printf("\n");
}
 
//P
void *Mat_vect_mult(void* rank) 
{
   int my_rank = *(int *) rank;
   int de_t = my_rank * M / NUM_OF_THREADS;
   int a_t = de_t + (M / NUM_OF_THREADS) - 1;
   int i, j;
   for (i = de_t; i <= a_t; i++) 
   {
      y[i] = 0.0;
      for (j = 0; j < M; j++)
      {
         y[i] += A[i*M+j]*x[j];
      }
   }
   return NULL;
} 

//S
void _Mat_vect_mult(void)
{ 
   int i,j;
   for(i = 0; i < M; i++)
   {
      y_s[i] = 0.0;
      for(j = 0; j < N; j++)
      {
  	 y_s[i] += A[i * N + j] * x[j];
      }
   }
}


int main(int argc, char* argv[]){

    timeval inicio;
    timeval fin;

    int fin_p, fin_s ; // long
    if(argc != 4)
    {
	fputs("Parametros son M N y NUM_OF_THREADS\n", stdout);
	return EXIT_FAILURE;
    }
    else
    {
	M = atoi(argv[1]);
	N = atoi(argv[2]);
	NUM_OF_THREADS = atoi(argv[3]);
    }

    //srand(time(NULL));

    int i, _error;

    pthread_t* threads = (pthread_t *)malloc(NUM_OF_THREADS * sizeof(pthread_t));

    A   = (float *) malloc(M * N * sizeof(float));
    x   = (float *) malloc(N * sizeof(float));
    y   = (float *) malloc(M * sizeof(float));
    y_s = (float *) malloc(M * sizeof(float));

    Llenar_matriz(A, M, N);

    Llenar_vector(x, N);

    gettimeofday(&inicio, 0);

    for(i = 0; i < NUM_OF_THREADS; i++)
    {
      _error = pthread_create(&threads[i], NULL, Mat_vect_mult, &i);
      if (_error)
      {
        fprintf(stderr, "error: pthread_create, rc: %d\n", _error);
	return EXIT_FAILURE;
      }
    }
    for (i = 0; i < NUM_OF_THREADS; i++)
    {
       pthread_join(threads[i], NULL);
    }
//P
    gettimeofday(&fin, 0);
    fin_p = Tiempo_segundos(inicio, fin);
//S
    gettimeofday(&inicio, 0);
    _Mat_vect_mult();

    gettimeofday(&fin, 0);
    fin_s = Tiempo_segundos(inicio, fin);
     

    fprintf(stdout, "SIZE: %d x %d AND %d THREADS\n", M, N, NUM_OF_THREADS);
    fprintf(stdout, "TIEMPO P :%d mseg\n", fin_p);
    fprintf(stdout, "TIEMPO S :%d mseg\n", fin_s);
 
    free(A);
    free(x);
    free(y);
    free(y_s);
    free(threads);
    pthread_mutex_destroy(&mutex);
 
}
