#include "mpi.h"
#include <math.h>
#include <stdio.h>

#define MESTRE 0
int main(int argc,char *argv[])
{
   int n, id, numprocs;
   double PI25DT = 3.141592653589793238462643;
   double pi_local, pi, h, soma, x;

   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
   MPI_Comm_rank(MPI_COMM_WORLD,&id);

   n = 0;

   if (id == MESTRE) { 
      printf("Entre com o número de intervalos: ");
      scanf("%d", &n);
   }

   MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
   h = 1.0 / n;
   soma = 0.0;
   for (int i = id + 1; i <= n; i+=numprocs){
	  x = h * ((double)i - 0.5); 
	  soma += (4.0 / (1.0 + x * x));
   }
   pi_local = h * soma;

   MPI_Reduce(&pi_local, &pi, 1, MPI_DOUBLE, MPI_SUM, MESTRE, MPI_COMM_WORLD);

  if (id == MESTRE){
     printf("pi é aproximadamente %.16f, erro é %.16f\n", pi, fabs(pi - PI25DT));
  } 
  MPI_Finalize();
}


