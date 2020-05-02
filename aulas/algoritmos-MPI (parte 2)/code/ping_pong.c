#include <mpi.h>
#include <stdio.h>

#define MESTRE 0
#define LIMITE 100

int main(int argc, char *argv[]){

   int id, vizinho, size, contador, tag = 0;	

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &id);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   
   vizinho = (id + 1) % 2;
   contador = 0;

   while (contador < LIMITE) {
   
      if (id == contador % 2) {
         contador++;
	 MPI_Send(&contador, 1, MPI_INT, vizinho, tag, MPI_COMM_WORLD);
         printf("%d enviou valor %d para %d\n", id, contador, vizinho);

      } else {
	 MPI_Recv(&contador, 1, MPI_INT, vizinho, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	 printf("%d recebeu valor %d de %d\n", id, contador, vizinho);
      }
   }


   MPI_Finalize();	
   return 0;
}
