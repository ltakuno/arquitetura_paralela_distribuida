#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

#define MASTER 0
#define n 16

#define max(a, b) ((a) > (b)) ? (a) : (b)
#define min(a, b) ((a) < (b)) ? (a) : (b)
int main(int argc, char *argv[]) {

   int id, p;
   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&p);
   MPI_Comm_rank(MPI_COMM_WORLD,&id);


   // id de processos
   int *pids = (int*) malloc(p*sizeof(int));

   // guarda os ids de processo na ordem reversa 
   for(int i = 0; i < p; i++){
      pids[i] = p - i - 1;   
   }

   MPI_Group group_world;
   MPI_Group reverse_group;
   MPI_Comm reverse_comm;

   MPI_Comm_group(MPI_COMM_WORLD, &group_world);
   MPI_Group_incl(group_world, p, pids, &reverse_group);
   MPI_Comm_create(MPI_COMM_WORLD, reverse_group, &reverse_comm);
  
   int input[n] = {3, 2, -7, 11, 10, -6, 4, 9, -6, 1, -2, -3, 4, -3, 0, 2};
   int q[n];
   int psum[n] = {0};
   int ssum[n] = {0};
   int pmax[n] = {0};
   int smax[n] = {0};

   
   int r = (n/p);

   int *fronteira = malloc(p * sizeof(int));
   
   // Distritua a entrada
   MPI_Scatter(input, r, MPI_INT, q, r, MPI_INT, MASTER, MPI_COMM_WORLD);
   MPI_Barrier(MPI_COMM_WORLD);

   // calcular a soma de prefixos de Q no array PSUM
   psum[0] = q[0];
   for (int i = 1; i < r; i++){
      psum[i] = q[i] + psum[i-1];
   }

   // calcular a soma de sufixo de Q no array SSUM
   ssum[r - 1] = q[r - 1];
   for (int i = r - 2; i >= 0; i--){
     ssum[i] = q[i] + ssum[i + 1];
   } 

   int psum_reduce = psum[r - 1];
   int ssum_reduce = ssum[0];
   int psum_recv = 0, ssum_recv = 0;
 
   MPI_Exscan(&psum_reduce, &psum_recv, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
   MPI_Exscan(&ssum_reduce, &ssum_recv, 1, MPI_INT, MPI_SUM, reverse_comm);
   if (id > MASTER){
      for (int i = 0; i < r; i++) {
         psum[i] += psum_recv;
      }
   }

   if (id < p - 1){
      for (int i = r - 1; i >= 0; i--){
         ssum[i] += ssum_recv;
      }
   }

   // Compute em paralelo o sufixo máximo de PSUM no vetor SMAX
   smax[r - 1] = psum[r - 1];
   for (int i = r - 2; i >= 0; i--){
      smax[i]  = max(psum[i], smax[i + 1]);
   }

   // Compute em paralelo o prefixo máximo de SSUM no vetor PMAX
   pmax[0] = ssum[0];
   for (int i = 1; i < r; i++){
      pmax[i] = max(ssum[i], pmax[i - 1]);
   }

   int pmax_reduce = pmax[r - 1];
   int smax_reduce = smax[0];
   int pmax_recv, smax_recv;
   MPI_Exscan(&pmax_reduce, &pmax_recv, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
   MPI_Exscan(&smax_reduce, &smax_recv, 1, MPI_INT, MPI_MAX, reverse_comm);

   if (id > MASTER) {
      for (int i = 0; i < r; i++) {
         pmax[i] = max(pmax_recv, pmax[i]);
      }
   }

   if (id < p - 1) {
      for (int i = r - 1; i >= 0; i--) {
         smax[i] = max(smax_recv, smax[i]);
      }
   }
	   
   int m[n] = {0};
   int ms[n] = {0};
   int mp[n] = {0};
   int maior_soma = INT_MIN;
   for (int i = 0; i < r; i++){
      ms[i] = pmax[i] - ssum[i] + q[i];	   
      mp[i] = smax[i] - psum[i] + q[i];
      m[i] = ms[i] + mp[i] - q[i];
      maior_soma = max(maior_soma, m[i]);
      printf("m[%d] = %d \n", id * r + i, m[i]); 
   }

   int maior_global;

   MPI_Allreduce(&maior_soma, &maior_global, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
   printf("maior global: %d\n", maior_global);

   free(fronteira);
   MPI_Finalize();
   return 0;
}
