#include<stdio.h>
#include<stdlib.h>

#define N 4

void multiplica(int *A, int *B, int *C){
   for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++){
         C[i * N + j] = 0;    
         for (int k = 0; k < N; k++)
	    C[i * N  + j] += A[i * N + k] * B[k * N + j]; 
      } 
}

void mostra_matriz(int *A){
   for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
         printf("%d ", A[i * N + j]);
      }
      printf("\n");
   }

}

void inicializa_exemplo(int *A, int *B){
   for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
         A[i * N + j] = 1;
	 B[i * N + j] = 1;
      }
   }

}

int main(int argc, char *argv[]) {
   int *A, *B, *C;

   A = (int *) malloc( N * N * sizeof(int));
   B = (int *) malloc( N * N * sizeof(int));
   C = (int *) malloc( N * N * sizeof(int));

   inicializa_exemplo(A, B);

   printf("Matriz A:\n");
   mostra_matriz(A);

   printf("Matriz B:\n");
   mostra_matriz(B);

   multiplica(A, B, C);

   printf("Matriz C:\n");
   mostra_matriz(C);

   free(A);
   free(B);
   free(C);
   return 0;
}
