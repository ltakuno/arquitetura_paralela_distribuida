#include<stdio.h>

int main(int argc, char *argv[]){

    FILE *arq;

    if (argc < 2){
       printf("uso: \n");
       printf("gera_mpi <nome_programa>.c\n");
       return 1;
    }

    arq = (FILE *)fopen(argv[1], "w");
    fprintf(arq, "#include<mpi.h>\n");
    fprintf(arq, "#include<stdio.h>\n");
    fprintf(arq, "#include<stdlib.h>\n\n");
    fprintf(arq, "#define MESTRE 0\n\n");
    fprintf(arq, "int main(int argc, char *argv[]) {\n\n");
    fprintf(arq, "   int id, p;\n");
    fprintf(arq, "   MPI_Init(&argc,&argv);\n");
    fprintf(arq, "   MPI_Comm_size(MPI_COMM_WORLD,&p);\n");
    fprintf(arq, "   MPI_Comm_rank(MPI_COMM_WORLD,&id);\n");
    fprintf(arq, "   MPI_Finalize();\n");
    fprintf(arq, "   return 0;\n");
    fprintf(arq, "}");
    fclose(arq);
    return 0;
}
