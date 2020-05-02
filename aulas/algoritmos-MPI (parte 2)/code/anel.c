#include<mpi.h>
#include<stdio.h>

#define MESTRE 0

int main(int argc, char *argv[]){
    int id, p;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    int token, tag = 0;

    if (id != MESTRE) {
       MPI_Recv(&token, 1, MPI_INT, id - 1, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       printf("%d recebeu token %d de %d\n", id, token, id-1);
    } else {
       token = 123;
    }

    MPI_Send(&token, 1, MPI_INT, (id + 1) % p, tag, MPI_COMM_WORLD);

    if (id == 0){
       MPI_Recv(&token, 1, MPI_INT, p - 1, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       printf("%d recebeu token %d de %d\n", id, token, p - 1);
    } 

    MPI_Finalize();

}


