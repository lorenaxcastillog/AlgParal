#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <sys/time.h>

int NUM_OF_THREADS, N;
pthread_mutex_t mutex;
double sum=0.0;
int flag=0;

long Tiempo_total(timeval inicio, timeval fin)
{
  long seg, useg;
  seg  = fin.tv_sec  - inicio.tv_sec;
  useg = fin.tv_usec - inicio.tv_usec;
  return ((seg) * 1000 + useg/1000.0) + 0.5;
}

//BW
void* Pi_BW(void * rank)
{
  int my_rank = *((int*)rank);
  double factor;
  long long i;
  long long my_n = N/NUM_OF_THREADS;
  long long my_first_i = my_n*my_rank;
  long long my_last_i  = my_first_i+my_n;
  double my_sum = 0.0;

  if((my_first_i % 2) == 0)
  {
     factor = 1.0;
  }
  else
  {
     factor = -1.0;
  }
  for(i = my_first_i; i < my_last_i; i++, factor =-factor)
  {
    my_sum += factor/(2*i+1);
  }
  while(flag!=my_rank);
  sum += my_sum;
  flag = (flag+1) % NUM_OF_THREADS;
}

//Mtx
void* Pi_Mtx(void * rank)
{
  int my_rank = *((int*)rank);
  double factor;
  long long i;
  long long my_n = N/NUM_OF_THREADS;
  long long my_first_i = my_n*my_rank;
  long long my_last_i = my_first_i+my_n;
  double my_sum = 0.0;

  if((my_first_i%2) == 0)
  {
     factor = 1.0;
  }
  else
  {
     factor = -1.0;
  }
  for(i = my_first_i; i < my_last_i; i++, factor =- factor)
  {
    my_sum += factor/(2*i+1);
  }
  pthread_mutex_lock(&mutex);
  sum += my_sum;
  pthread_mutex_unlock(&mutex);
}

int main(int argc, char* argv[])
{
   N = atoi(argv[1]);
   NUM_OF_THREADS = atoi(argv[2]);
   timeval inicio;
   timeval fin;
   long fin_p;
   srand(time(NULL));
   int i;
   pthread_t* threads = (pthread_t *)malloc(NUM_OF_THREADS * sizeof(pthread_t));
   gettimeofday(&inicio, 0);
   for(i = 0; i < NUM_OF_THREADS; i++)
   {
      pthread_create(&threads[i], NULL,Pi_Mtx,&i);
      pthread_join(threads[i], NULL);
   }
   gettimeofday(&fin, 0);
   fin_p = Tiempo_total(inicio, fin);
   fprintf(stdout, "TIEMPO P : %d mseg\n",(int) fin_p);
   free(threads);
}
