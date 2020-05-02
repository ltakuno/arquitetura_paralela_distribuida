#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 40 
#define MESTRE 0
#define MAX 100

/* prototipação */
void ordenacao_par_impar(int a[], int n, int id, int p);
int compare(const void *a, const void *b); 
void copiar(int w[], int v[], int n);
 

void gerar(int v[], int n){
   srand(time(NULL));
   for (int i = 0; i < n; i++)
      v[i] = rand() % MAX;
}

void mostrar(int v[], int n){
   for(int i = 0; i < n; i++)
	  printf("%d ", v[i]); 
   printf("\n");
}

void testar_ordenado(int v[], int n){
   int ok = 1;
   for (int i = 0; i < n-1; i++)
	   if (v[i] > v[i+1])
		   ok = 0;
   if (!ok)
      printf("Erro no algoritmo de ordenação!\n");  
}

void copiar(int w[], int v[], int n){
   for (int i = 0; i < n; i++)
      w[i] = v[i];
} 

void compara_resultado(int v[], int w[], int n) {
   int ok = 1;
   for (int i = 0; i < n; i++)
      if (v[i] != w[i])
	      ok = 0;

   if (!ok){
      printf("Erro no algoritmo de ordenação! Erro na comparação\n");
      mostrar(v, n);
      mostrar(w, n);
   }
}

int main(int argc, char *argv[]){

   int id, p, n = N, v[N], v_local[N], n_local;
   int w[N];

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &id);
   MPI_Comm_size(MPI_COMM_WORLD, &p);

   n_local = n / p;

   if (id == MESTRE){
       gerar(v, n);
       mostrar(v, n);
       copiar(w, v, n);
       qsort(w, n, sizeof(int), compare);
   }

   MPI_Scatter(v, n_local, MPI_INT, v_local, n_local, MPI_INT, MESTRE, MPI_COMM_WORLD);

   MPI_Barrier(MPI_COMM_WORLD);
   ordenacao_par_impar(v_local, n_local, id, p);

   MPI_Gather(v_local, n_local, MPI_INT, v, n_local, MPI_INT, MESTRE, MPI_COMM_WORLD);

   if (id == MESTRE) {
      printf("Ordenado: \n");
      mostrar(v, n);
      testar_ordenado(v, n);
      compara_resultado(v, w, n);
   }
   MPI_Finalize();
}

int compare(const void *a, const void *b) {
   return (*(int*)a - *(int*)b);
}


int computar_vizinho(int id, int p, int fase){
   int vizinho;

   if (fase % 2 == 0) /* fase par */
      if (id % 2 != 0) /* id impar */
	      vizinho = id - 1;
      else             /* id par */
	      vizinho = id + 1;
   else               /* fase impar */
      if (id % 2 != 0) /* id impar */
	      vizinho = id + 1; 
      else            /* id par */
	      vizinho = id - 1;

   if (vizinho == -1 || vizinho == p)
	   vizinho = MPI_PROC_NULL;

   return vizinho;
}

void depura(int fase, int id, int v[], int n){
   for (int i = 0; i < n; i++)
      printf("fase %d, id = %d,  %d\n",fase, id, v[i]);
}



void intercala_e_mantem_menores_valores(int a[], int b[], int c[], int n){
   int i, j, k;
   
   i = j = k = 0;
   while (k < n) { 
      if (a[i] <= b[j]) c[k++] = a[i++];
      else              c[k++] = b[j++];
   }

   for (i = 0; i < n; i++)
      a[i] = c[i];
}

void intercala_e_mantem_maiores_valores(int a[], int b[], int c[], int n){
   int i, j, k;

   i = j = k = n - 1;
   while (k >= 0) { 
      if (a[i] >= b[j]) c[k--] = a[i--];
      else              c[k--] = b[j--];
   }

   for (i = 0; i < n; i++)
      a[i] = c[i];
}

void ordenacao_par_impar(int a[], int n, int id, int p){
   int vizinho;
   int b[N], c[N];

   qsort(a, n, sizeof(int), compare);
   
   for (int fase = 0; fase < p; fase++){
      vizinho = computar_vizinho(id, p, fase);
      if (vizinho >= 0) { // não está ocioso
         MPI_Sendrecv(a, n, MPI_INT, vizinho, 0, b, n, MPI_INT, vizinho, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
         if (id < vizinho)
            intercala_e_mantem_menores_valores(a, b, c, n);
         else
            intercala_e_mantem_maiores_valores(a, b, c, n);
      }
   }
}	

