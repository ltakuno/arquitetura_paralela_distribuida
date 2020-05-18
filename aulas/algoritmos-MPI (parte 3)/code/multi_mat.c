#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define MESTRE 0

#define N 4

void multiplica(int *A, int *B, int *C, int n){
   for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++){
         C[i * n + j] = 0;    
         for (int k = 0; k < n; k++){
	    C[i * n  + j] += A[i * n + k] * B[k * n + j]; 
	 }   
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


void decompoe_e_envia_matriz(int *m, int *bloco_x, int n, int tam_bloco, MPI_Comm rol_comm, MPI_Comm col_comm, int coord[]) { 
   int *matriz_linha = (int *) malloc(tam_bloco * n * sizeof(int));
   if (coord[1] == 0) {
      MPI_Scatter(m, tam_bloco * n, MPI_INT, matriz_linha, tam_bloco * n, MPI_INT, 0, col_comm);
   }

   for (int i = 0; i < tam_bloco; i++) { 
      MPI_Scatter(&matriz_linha[i * tam_bloco], tam_bloco, MPI_INT, &bloco_x[i * tam_bloco], tam_bloco,  MPI_INT, 0, rol_comm);
   
   } 
   free(matriz_linha);
}

void testa_blocos(int *bloco, int tam_bloco, char str[]){
   int id, p;	
   MPI_Barrier(MPI_COMM_WORLD);
   MPI_Comm_rank(MPI_COMM_WORLD, &id);
   MPI_Comm_size(MPI_COMM_WORLD, &p);

   MPI_Barrier(MPI_COMM_WORLD);
 
   if (id  == MESTRE){
      printf("%s\n", str);
   }
   MPI_Barrier(MPI_COMM_WORLD);
 
   for (int i = 0; i < p; i++){ 
      if (i == id) {	   
         printf("id = %d\n" , id);
         mostra_matriz(bloco, tam_bloco);
      }	 

      MPI_Barrier(MPI_COMM_WORLD);
   }
   
}


void distribui_dados(int *A, int *B, 
		     int *bloco, int *bloco_B, 
		     int n, int tam_bloco,  
		     MPI_Comm row_comm, MPI_Comm col_comm, int coord[]){
 
  decompoe_e_envia_matriz(A, bloco  , n, tam_bloco, row_comm, col_comm, coord);
  decompoe_e_envia_matriz(B, bloco_B, n, tam_bloco, row_comm, col_comm, coord);
}


void envia_bloco_A(int i, int *bloco_A, int *bloco, int tam_bloco, int coord[], int tam_grid, MPI_Comm row_comm){
   int pivot = (coord[0] + i) % tam_grid;
   if (coord[1] == pivot){
      for(int i = 0; i < tam_bloco * tam_bloco; i++) { 
         bloco_A[i] = bloco[i];	      
      }
   }
   MPI_Bcast(bloco_A, tam_bloco * tam_bloco, MPI_INT, pivot, row_comm);
}

void envia_bloco_b_com_deslocamento_ciclico(int *bloco_B, int tam_bloco, MPI_Comm col_comm, int coord[], int tam_grid){
   MPI_Status status;
   int prox = coord[0] + 1;
   if (coord[0] == tam_grid-1) prox = 0;
   int ant = coord[0] - 1;
   if (coord[0] == 0) ant = tam_grid-1;
   MPI_Sendrecv_replace(bloco_B, tam_bloco * tam_bloco, MPI_INT, prox, 0, ant, 0, col_comm, &status); // envia vertical (coluna)
}

void multiplica_paralelo_metodo_de_fox(int *bloco_A, int *bloco, int *bloco_B, int *bloco_C, int tam_bloco, int tam_grid, int coord[], MPI_Comm row_comm, MPI_Comm col_comm){
   int id;

   MPI_Comm_rank(MPI_COMM_WORLD, &id);

   for (int i = 0; i < tam_grid; i++){
      // 1. envia blocos da matriz para os processos da linha do grid
      envia_bloco_A(i, bloco_A, bloco, tam_bloco, coord, tam_grid, row_comm);    

      // 2. multiplicação de blocos.
      multiplica(bloco_A, bloco_B, bloco_C, tam_bloco);

      // 3. deslocamento cíclico da matriz B na coluna do grid  
      envia_bloco_b_com_deslocamento_ciclico(bloco_B, tam_bloco, col_comm, coord, tam_grid);
  }
}

void coletar_resultados(int *C, int *bloco_C, int n, int tam_bloco,int coord[], MPI_Comm row_comm, MPI_Comm col_comm){
   int *resultado_linha = (int *) malloc(n * tam_bloco * sizeof(int));

   for (int i = 0; i < tam_bloco; i++) {
      MPI_Gather(&C[i * tam_bloco], tam_bloco, MPI_INT, &resultado_linha[i * n], tam_bloco, MPI_INT, 0, row_comm);   
   }

   if (coord[1] == 0){
      MPI_Gather(resultado_linha, tam_bloco * n, MPI_INT, C, tam_bloco * n, MPI_INT, 0, col_comm);
   }

   free(resultado_linha);   
}

int main(int argc, char *argv[]) {
   int p, id, tam_grid;
   MPI_Comm comm, row_comm, col_comm;
   int dim[2], periodico[2], coord[2], subdim[2];

   MPI_Init(&argc,  &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &id);
   MPI_Comm_size(MPI_COMM_WORLD, &p);

   tam_grid = sqrt(p); // numero de linhas/colunas da grid (fazer checagem)
   dim[0] = dim[1] = tam_grid;
   periodico[0] = periodico[1] = 1;

   // Criando o comunicador cartesiano
   MPI_Cart_create(MPI_COMM_WORLD, 2, dim, periodico, 1, &comm);

   // Determina coordenadas cartesianas para todos os processos
   MPI_Cart_coords(comm, id, 2, coord);

   // Criando comunicadores para linha
   subdim[0] = 0; // dimensão fixa
   subdim[1] = 1; // dimensão pertence ao subgrid
   MPI_Cart_sub(comm, subdim, &row_comm);

   // Criando comunicadores para linha
   subdim[0] = 1; // dimensão pertence ao subgrid
   subdim[1] = 0; // dimensão fixa
   MPI_Cart_sub(comm, subdim, &col_comm);

   
   int *bloco, tam_bloco, *bloco_A, *bloco_B, *bloco_C;	
   int *A, *B, *C;
   tam_bloco = N / tam_grid;
   bloco_A = (int *) malloc(tam_bloco * tam_bloco * sizeof(int)); 
   bloco_B = (int *) malloc(tam_bloco * tam_bloco * sizeof(int)); 
   bloco_C = (int *) malloc(tam_bloco * tam_bloco * sizeof(int)); 
   bloco   = (int *) malloc(tam_bloco * tam_bloco * sizeof(int)); 

   if (id == MESTRE){
      A = (int *) malloc(N * N * sizeof(int));
      B = (int *) malloc(N * N * sizeof(int));
      C = (int *) malloc(N * N * sizeof(int));
      inicializa_exemplo(A, B);

      printf("Matriz A:\n");
      mostra_matriz(A, N);

      printf("Matriz B:\n");
      mostra_matriz(B, N);
   }
   
   distribui_dados(A, B, bloco, bloco_B, N, tam_bloco, row_comm, col_comm, coord);
   multiplica_paralelo_metodo_de_fox(bloco_A, bloco, bloco_B, bloco_C, tam_bloco, tam_grid, coord, row_comm, col_comm);

//   testa_blocos(bloco, tam_bloco, "blocos de A");
//   testa_blocos(bloco_B, tam_bloco, "blocos de B");
//   testa_blocos(bloco_C, tam_bloco, "Resultado");            

   coletar_resultados(C, bloco_C, N, tam_bloco, coord, row_comm, col_comm);

   if (id == MESTRE){
      printf("Resultado: \n");
      mostra_matriz(C, N);
   }

   if (id == MESTRE){
      free(A);
      free(B);
      free(C);
   }
   free(bloco_A);
   free(bloco_B);
   free(bloco_C);
   free(bloco);
   return 0;
}
