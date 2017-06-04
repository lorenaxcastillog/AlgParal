#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"  

int main(int argc, char* argv[])
{
  int p, my_rank; 
  int max_p = 8;
  int tam_buffer = 24; /* tamaño del buffer a enviar,tiene que ser divisible por 2, 4, 6 y 8 */
  int tag = 5; //Etiqueta S=5
  MPI_Status status;

  int enviar[tam_buffer]; int recibir[tam_buffer];   

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  if (p > max_p || p % 2 != 0) 
  {
    if (my_rank == 0)
    {
      printf("Ingresa un número par de procesos (al menos %d)\n", max_p);
    }
    MPI_Finalize();
    exit(0);
  }

  for ( int i = 0; i < tam_buffer/p; i++ )
  {
     enviar[i] = (tam_buffer/p) * my_rank + i;
  }

  if (my_rank == 0) {    /* Process 0 does this */
    
    /* Gather the array x from all proceses, place it in y */
    MPI_Gather(enviar, tam_buffer/p, MPI_INT, recibir, tam_buffer/p, MPI_INT, 0, MPI_COMM_WORLD);

    /* Print out the gathered array */
    for ( int i = 0; i < tam_buffer; i++) 
    {
      printf(" %d", recibir[i]);
    }
    printf("\n\n");

    for ( int i =0; i < tam_buffer/p; i++ )
    {
      printf (" %d", enviar[i]);
    }
    printf ("\n");

    /* Receive messages with hostname and the original data */
    /* from all other processes */
    for ( int i = 1; i < p; i++ )
    { 
      MPI_Recv (&recibir, tam_buffer/p, MPI_INT, i, tag, MPI_COMM_WORLD, &status);
      for ( int j = 0; j < tam_buffer/p; j++ )
      {
	printf(" %d", recibir[j]);
      }
      printf("\n");
    }
    
    printf("Fin\n");
  } 
  else 
  { /* all other processes do this */

    /* Receive the scattered array from process 0, place it in array y */
    MPI_Gather(&enviar, tam_buffer/p, MPI_INT, &recibir, tam_buffer/p, MPI_INT, 0, MPI_COMM_WORLD);
 
    /* Send the received array back to process 0 */
    MPI_Send (&enviar, tam_buffer/p, MPI_INT, 0, tag, MPI_COMM_WORLD);

  }

  MPI_Finalize();
  exit(0);
}

