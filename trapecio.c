#include <mpi.h>
#include <stdio.h>
#include <string.h>

float trapecio(float local_a, float local_b, int local_n, float h)
{
    float integral;
    float x;
    int i;
    integral = ( ( f(local_a) + f(local_b) ) *1.0 )/2.0;
    x = local_a;

    for( i = 1; i <= (local_n-1); i++ )
    {
        x+=h;
        integral+=f(x);
    }
    integral*=h;
    return integral;
}

float f(float x)
{
  float return_val=x*x;
  return return_val;
}

void Get_data(int my_rank,int p,float *a_ptr,float *b_ptr,int *n_ptr)
{
      int source=0 , dest , tag;
      MPI_Status status;
      if(my_rank==0)
      {
          printf("Enter a, b, and n\n");
          scanf("%f %f %d",a_ptr,b_ptr,n_ptr);
          for(dest=1;dest<p;dest++) {
            tag=30;
            MPI_Send(a_ptr,1,MPI_FLOAT,dest,tag,MPI_COMM_WORLD);
            tag=31;
            MPI_Send(b_ptr,1,MPI_FLOAT,dest,tag,MPI_COMM_WORLD);
            tag=32;
            MPI_Send(n_ptr,1,MPI_INT,dest,tag,MPI_COMM_WORLD);
          }
      }
       else {
          tag=30;
          MPI_Recv(a_ptr,1,MPI_FLOAT,source,tag,MPI_COMM_WORLD,&status);
          tag=31;
          MPI_Recv(b_ptr,1,MPI_FLOAT,source,tag,MPI_COMM_WORLD,&status);
          tag=32;
          MPI_Recv(n_ptr,1,MPI_INT,source,tag,MPI_COMM_WORLD,&status);
        }
}


int main(int argc,char **argv)
{
   int  my_rank , p , n , local_n , source , dest=0 , tag=50;
   float a , b ,  h , local_a , local_b, integral , total;
   //
   MPI_Status status;
   MPI_Init(&argc,&argv);
   MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
   MPI_Comm_size(MPI_COMM_WORLD,&p);
   //
   Get_data(my_rank,p,&a,&b,&n);
   h=(b-a)/n ; local_n=n/p;
   local_a=a+my_rank*local_n*h ;
   local_b=local_a+local_n*h;
   integral=trapecio(local_a,local_b,local_n,h);
   if(my_rank==0) {
     total=integral;
     for(source=1;source<p;source++) {
       MPI_Recv(&integral,1,MPI_FLOAT,source,tag,MPI_COMM_WORLD,&status);
       total+=integral;
       printf("proceso %d calculo un total de %f\n",source,total);
     }
      printf("con n= %d trapezoides\n la estimacion",n);
      printf("de la integral entre %f y %f\n es= %f \n",a,b,total);
    }
    else {
      MPI_Send(&integral,1,MPI_FLOAT,dest,tag,MPI_COMM_WORLD);
    }
    MPI_Finalize();
}
