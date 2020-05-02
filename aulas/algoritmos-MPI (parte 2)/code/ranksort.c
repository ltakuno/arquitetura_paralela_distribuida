#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define MESTRE 0
#define N 3000
#define MAX (10 * N)

int contem(int v[], int n, int x){
   for (int i = 0; i < n; i++){
       if (v[i] == x){
	       return 1;
       }
   }
   return 0;
}
void gerar_vetor_sem_repeticao(int v[], int n){
   int x, vezes = 1;
  for (int i = 0; i < n; i++){ 
      do {   
         srand(time(NULL) + i*vezes);
         x = rand() % MAX;
	 vezes++;
      } while (contem(v, i, x));
      v[i] = x;
   }
}

void mostrar_vetor(int v[],int n) {
   for (int i = 0; i < n; i++){
      printf("%d ", v[i]);
   }
   printf("\n");
}

void copiar_vetor(int w[], int v[], int n){
   for (int i = 0; i < n; i++)
	   w[i] = v[i];
}

int compare(const void *a, const void *b) {
   return (*(int*)a - *(int*)b);
}

void compara_resultado(int v[], int w[], int n) {
   int ok = 1;
   for (int i = 0; i < n; i++)
      if (v[i] != w[i])
	      ok = 0;

   if (!ok){
      printf("Erro no algoritmo de ordenação! Erro na comparação\n");
      mostrar_vetor(v, n);
      mostrar_vetor(w, n);
   }
}

void testar_ordenado(int v[], int n){
   int ok = 1;
   for (int i = 0; i < n-1; i++)
	   if (v[i] > v[i+1])
		   ok = 0;
   if (!ok)
      printf("Erro no algoritmo de ordenação!\n");
}

int main(int argc, char* argv[]){
   int id, p, n = N, v[N], vetor_ordenado[N], v_local, n_local;
   int contador = 0, contador_recv = 0, posicao, valor;
   char buffer[100];
   int w[N];

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &id);
   MPI_Comm_size(MPI_COMM_WORLD, &p);

   if (id  == MESTRE){
	  gerar_vetor_sem_repeticao(v, n);
	  copiar_vetor(w, v, n);
	  qsort(w, n, sizeof(int), compare);
   }

   MPI_Bcast(&v, n, MPI_INT, 0, MPI_COMM_WORLD);
   n_local = n/(p-1);       // desconsiderar o mestre

   if (id != MESTRE){
      int inicio = (id - 1)* n_local;
      int fim  = inicio + n_local;
      for (int k = inicio; k < fim; k++) {
         valor = v[k];
	 contador = 0;   
         for (int i = 0; i < n; i++){
	     if (v[i] < valor)
		    contador++; 
	 }
	 posicao = 0;
         MPI_Pack(&contador, 1, MPI_INT, buffer, 100, &posicao, MPI_COMM_WORLD);
	 MPI_Pack(&valor, 1, MPI_INT, buffer, 100, &posicao, MPI_COMM_WORLD);
	 MPI_Ssend(buffer, 100, MPI_PACKED, MESTRE, 0, MPI_COMM_WORLD);
      } 
   } else {
	   
      for (int k = 0; k < n; k++) {
         MPI_Recv(buffer, 100, MPI_PACKED, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         posicao = 0;
         MPI_Unpack(buffer, 100, &posicao, &contador, 1, MPI_INT, MPI_COMM_WORLD);
         MPI_Unpack(buffer, 100, &posicao, &valor, 1, MPI_INT, MPI_COMM_WORLD);
	 vetor_ordenado[contador] = valor;
      }

      mostrar_vetor(vetor_ordenado, n);
      testar_ordenado(vetor_ordenado, n);
      compara_resultado(vetor_ordenado, w, n);
   }


   MPI_Finalize();
}
