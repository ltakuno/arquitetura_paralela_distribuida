#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

#define N 16 
#define MESTRE 0

// declaração de variáveis
int buffer[100];


// declaração de funções

void multiplica(int *A, int *B, int *C, int n){
   for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++){
         C[i * n + j] = 0;    
         for (int k = 0; k < n; k++)
	    C[i * n  + j] += A[i * n + k] * B[k * n + j]; 
      } 
}

void mostra_matriz(int *A, int n){
   for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
         printf("%d ", A[i * n + j]);
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

void envia(int i, int j, int destino) {
   int tag = 33;
   int posicao = 0;
   MPI_Request request;    
   MPI_Pack(&i, 1, MPI_INT, buffer, 100, &posicao, MPI_COMM_WORLD);
   MPI_Pack(&j, 1, MPI_INT, buffer, 100, &posicao, MPI_COMM_WORLD);
   MPI_Isend(buffer, 100, MPI_PACKED, destino, tag, MPI_COMM_WORLD, &request);
}

void recebe(int *i, int *j){
   int tag = 33;
   int posicao = 0;

   MPI_Recv(buffer, 100, MPI_PACKED, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
   MPI_Unpack(buffer, 100, &posicao, i, 1, MPI_INT, MPI_COMM_WORLD);
   MPI_Unpack(buffer, 100, &posicao, j, 1, MPI_INT, MPI_COMM_WORLD);
}

void envia_resultado(int i, int j, int r){
   int tag = 44;
   int posicao = 0;
   MPI_Request request; 
   
   MPI_Pack(&i, 1, MPI_INT, buffer, 100, &posicao, MPI_COMM_WORLD);
   MPI_Pack(&j, 1, MPI_INT, buffer, 100, &posicao, MPI_COMM_WORLD);
   MPI_Pack(&r, 1, MPI_INT, buffer, 100, &posicao, MPI_COMM_WORLD);
   MPI_Isend(buffer, 100, MPI_PACKED, MESTRE, tag, MPI_COMM_WORLD, &request);
}

void recebe_resultado(int *i, int *j, int *r){
   int tag = 44;
   int posicao = 0;

   MPI_Recv(buffer, 100, MPI_PACKED, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
   MPI_Unpack(buffer, 100, &posicao, i, 1, MPI_INT, MPI_COMM_WORLD);
   MPI_Unpack(buffer, 100, &posicao, j, 1, MPI_INT, MPI_COMM_WORLD);
   MPI_Unpack(buffer, 100, &posicao, r, 1, MPI_INT, MPI_COMM_WORLD);
}

int multiplica_linha_coluna(int *A, int *B, int linha, int coluna, int n){
   int r = 0;

   for (int i = 0; i < n; i++) { 
      r += A[linha * n + i] * B[i * n + coluna];	   
   }

   return r;
}

void copia_linha(int l, int *A, int *linha, int n) {
   for(int i = 0; i < n; i++){
      linha[i] = A[l * n + i];
   }
}

void copia_coluna(int c, int *A, int *coluna, int n) {
   for(int i = 0; i < n; i++){
      coluna[i] = A[i * n + c];
   }
}

void mostra_vetor(int *v, int n){
   for (int i = 0; i < n; i++) { 
      printf("%d ", v[i]);
   }
   printf("\n");
}
		


int main(int argc, char *argv[]) {
   int id, p;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &id);
   MPI_Comm_size(MPI_COMM_WORLD, &p);
   int *A, *B, *C;

   A = (int *) malloc( N * N * sizeof(int));
   B = (int *) malloc( N * N * sizeof(int));
 
   if (id == MESTRE) { 
      C = (int *) malloc( N * N * sizeof(int));
      inicializa_exemplo(A, B);
   }

   MPI_Bcast(A, N * N, MPI_INT, MESTRE, MPI_COMM_WORLD);
   MPI_Bcast(B, N * N, MPI_INT, MESTRE, MPI_COMM_WORLD);

   if (id == MESTRE) {
      int processo = 0;
     
      for (int i = 0; i < N; i++){
         for (int j = 0; j < N; j++) {	      
            if (processo % 4 == 0) { 
	      C[i*N + j] = multiplica_linha_coluna(A, B, i, j, N);
	    } else {
	      envia(i, j, processo % 4); 
            }
	    processo++;
	 }
         
	 processo = 0;
	 for (int j = 0; j < N; j++) {
	    if (processo % 4 != 0) {
	       int l, c, r;
	       recebe_resultado(&l, &c, &r);
	       C[l * N + c] = r;
	    }
	    processo++;
	 }
      }
   } else {
      int i, j;
      for (int k = 0; k < N*N; k += 4){
         recebe(&i, &j);
         int r = multiplica_linha_coluna(A, B, i, j, N);
	 envia_resultado(i, j, r);
      }
   }
   MPI_Barrier(MPI_COMM_WORLD);  
   if (id == MESTRE){   
      printf("Matriz C:\n");
      mostra_matriz(C, N);
      free(A);
      free(B);
      free(C);
   }

   return 0;
}
