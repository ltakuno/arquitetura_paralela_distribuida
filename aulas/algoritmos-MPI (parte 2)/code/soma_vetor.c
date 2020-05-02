#include <mpi.h>
#include <stdio.h>
#define MESTRE 0
#define N 40

void inicializa(int v[], int n, int valor_padrao){
   for (int i = 0; i < n; i++){
      v[i] = valor_padrao;
   }
}

void imprime(int id, int v[], int n){
   for (int i = 0; i < n; i++){
      printf("id %d - v[%d] = %d\n", id, i, v[i]); 
   } 
}

int main(int argc, char *argv[]){

   MPI_Init(&argc, &argv);

   int id, p, r;
   int a[N], b[N], resultado[N];
   int local_a[N], local_b[N], local_resultado[N];
   MPI_Comm_size(MPI_COMM_WORLD, &p);
   MPI_Comm_rank(MPI_COMM_WORLD, &id);
   r = N / p; 

   if (id == MESTRE) {
      inicializa(a, N, 1);
      inicializa(b, N, 2);
   } 
   MPI_Scatter(a, r, MPI_INT, local_a, r, MPI_INT, MESTRE, MPI_COMM_WORLD);
   MPI_Scatter(b, r, MPI_INT, local_b, r, MPI_INT, MESTRE, MPI_COMM_WORLD);

   for (int i = 0; i < r; i++) {
	   local_resultado[i] = local_a[i] + local_b[i];
   }

   MPI_Gather(local_resultado, r, MPI_INT, resultado, r, MPI_INT, MESTRE, MPI_COMM_WORLD);

   if (id == MESTRE) { 
      for (int i = 0; i < N; i++)
	     printf("%d ", resultado[i]);
      printf("\n");
   }

   MPI_Finalize();

   return 0;
}
