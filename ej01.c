#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) 
{
  //inicializar
  int total_pingpong = 10;
  int ranking_proceso_ping;
  int numero_procesos;

  MPI_Init(NULL, NULL);
//ranking de procesos
  MPI_Comm_rank(MPI_COMM_WORLD, &ranking_proceso_ping);
 
//tamño grupo asociado al comunicador
  MPI_Comm_size(MPI_COMM_WORLD, &numero_procesos);
 
  int contador = 0;
  int ranking_proceso_pong = (ranking_proceso_ping + 1) % 2; // 0 - 1 
  
  if(ranking_proceso_ping == 0)
  {
    if(contador <  total_pingpong)
    {
      contador++;
//int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
      MPI_Send(&contador, 1, MPI_INT, ranking_proceso_pong, 0, MPI_COMM_WORLD);

      printf("%d envió   %d  a %d\n",ranking_proceso_ping, contador, ranking_proceso_pong );
    }
   }

  while (contador < total_pingpong) 
  {
    if (ranking_proceso_ping == contador % 2) //odd
    {
      contador++;
      MPI_Send(&contador, 1, MPI_INT, ranking_proceso_pong, 0, MPI_COMM_WORLD); //1-tamaño variable
      printf("%d envió   %d  a %d\n",ranking_proceso_ping, contador, ranking_proceso_pong );
    }
    else
    {
//int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag,MPI_Comm comm, MPI_Status *status)

      MPI_Recv(&contador, 1, MPI_INT, ranking_proceso_pong, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("%d recibió %d  de %d\n",ranking_proceso_ping, contador, ranking_proceso_pong );
    }
  }
  MPI_Finalize();
}

//mpicc ej01.c -o ej

//mpirun -np 2 ./ej



