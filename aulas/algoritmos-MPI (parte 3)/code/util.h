#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


#define MAX 3000


void testar_ordenado(int v[], int n){
   int ok = 1;
   for (int i = 0; i < n-1; i++)
           if (v[i] > v[i+1])
                   ok = 0;
   if (!ok)
      printf("Erro no algoritmo de ordenação!\n");
}


void copiar_vetor(int w[], int v[], int n){
   for (int i = 0; i < n; i++)
           w[i] = v[i];
}

int compare(const void *a, const void *b) {
   return (*(int*)a - *(int*)b);
}

void mostrar_vetor(int v[], int n) {
   for (int i = 0; i < n; i++){
      printf("%d ", v[i]);
   }
   printf("\n");
}

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


