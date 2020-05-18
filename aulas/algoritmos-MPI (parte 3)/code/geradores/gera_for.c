#include<stdio.h>

int main(int argc, char *argv[]) {
   if (argc < 3){
      printf("uso: gera_for <variavel> <valor_limite> \n");    
   }
   printf("for (int %s = 0; %s < %s; %s++) {}\n", argv[1], argv[1], argv[2], argv[1]);

   return 0;
}
