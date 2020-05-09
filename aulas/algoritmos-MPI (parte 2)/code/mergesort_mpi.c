#include<mpi.h>
#include "util.h"

#define MESTRE 0
#define N 10

void merge(int v[], int n){
   int i = 0;
   int j = N/2; 
   int m = N/2;

   int w[N], k = 0;

   while (i < m && j < n){
      if (v[i] < v[j]) w[k++] = v[i++];
      else if (v[i] >= v[j]) w[k++] = v[j++];
   }

   while(i < m) w[k++] = v[i++];
   while(j < n) w[k++] = v[j++];
   for (int i = 0; i < n; i++) v[i] = w[i];
}

int main(int argc, char *argv[]) {

   int id, p, tag = 0;
   int v[N], w[N];
   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&p);
   MPI_Comm_rank(MPI_COMM_WORLD,&id);


   if (id == MESTRE){
      gerar_vetor_sem_repeticao(v, N);
      copiar_vetor(w, v, N);
      qsort(w, N, sizeof(int), &compare);
      MPI_Send(&v[N/2], N/2, MPI_INT, 1, tag, MPI_COMM_WORLD);
      qsort(v, N/2, sizeof(int), &compare);
      MPI_Recv(&v[N/2], N/2, MPI_INT, 1, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      merge(v, N);
      testar_ordenado(v, N);
      compara_resultado(v, w, N);
      mostrar_vetor(v, N);
   } else {
      MPI_Recv(v, N/2, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


      qsort(v, N/2, sizeof(int), &compare);
      MPI_Send(v, N/2, MPI_INT, 0, tag, MPI_COMM_WORLD);   
   }

   MPI_Finalize();
   return 0;
}
