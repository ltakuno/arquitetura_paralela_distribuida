#include<stdio.h>

int main(int argc, char *argv[]){

    FILE *arq;

    if (argc < 2){
       printf("uso: \n");
       printf("gera_prog <nome_programa>.c\n");
       return 1;
    }

    arq = (FILE *)fopen(argv[1], "w");
    fprintf(arq, "#include<stdio.h>\n\n");
    fprintf(arq, "int main(int argc, char *argv[]) {\n\n");
    fprintf(arq, "   return 0;\n");
    fprintf(arq, "}");
    fclose(arq);
    return 0;
}
