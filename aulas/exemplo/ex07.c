#include <omp.h>
#include <stdio.h>


#define MAX 100000
int main(){
   int i, j, A[MAX];

   #pragma omp parallel for
   for (i = 0; i < MAX; i++){
      int j = 5 + 2 * (i+1);
      A[i] = j;
   }


   for (i = 0; i < MAX; i++)
      printf("%d ", i);
   return 0;
}
