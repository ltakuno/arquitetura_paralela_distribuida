#include<mpi.h>
#include<stdio.h>
#define MESTRE 0

int main(int argc, char *argv[]){
   int id, p, a, b, tag = 0;
  
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &id);
   MPI_Comm_size(MPI_COMM_WORLD, &p);

   if (id == MESTRE) { 
      a = 0;
      b = 1;
   } else {
      a = 1;
      b = 0;
   }

   if (id == MESTRE) {
      MPI_Sendrecv(&a, 1, MPI_INT, 1, tag, 
		   &b, 1, MPI_INT, 1, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
   } else {
      MPI_Sendrecv(&b, 1, MPI_INT, 0, tag,  
		   &a, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
   printf("id = %d, a = %d, b = %d\n", id, a, b);
   MPI_Finalize();
}
