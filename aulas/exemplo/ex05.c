#include<omp.h>
#include<stdio.h>

int main(){

  int a[10] = {1,1,1,1,1,1,1,1,1,1};
  int b[10] = {2,2,2,2,2,2,2,2,2,2};

  int c[10], i;


  #pragma omp parallel for 
  for (i = 0; i < 10; i++)
     c[i] = a[i] + b[i];


  for (i = 0; i < 10; i++)
    printf("%d  ", c[i]);


  printf("\n");




   return 0;
}
