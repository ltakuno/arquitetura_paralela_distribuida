#include<stdio.h>

int main(int argc, char *argv[]) {
   if (argc < 5){
      printf("uso: gera_for2 <variavel1> <valor_limite1> <variavel2> <valor_limite2>\n");    
      return 1;
   }
  printf("for (int %s = 0; %s < %s; %s++) {\n", argv[1], argv[1], argv[2], argv[1]);
  printf("for (int %s = 0; %s < %s; %s++) {}\n}\n", argv[3], argv[3], argv[4], argv[3]);

   return 0;
}
