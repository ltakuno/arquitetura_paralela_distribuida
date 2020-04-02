#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define size 100000

omp_lock_t lock[size];
int main () {
   int a[size], i = 0, r, temp;
   for ( i=0; i<size; i++ ) {
      omp_init_lock(&lock[i]);
   }
   #pragma omp parallel shared(a) num_threads(4)
   {
      #pragma omp for schedule(static)
         for ( i=0; i<size; i++ ) {
            a[i] = i;
         }
      #pragma omp for schedule(static)
         for ( i=0; i<size; i++ ) {
            int r = rand()%size;
            if (i!=r) {
               omp_set_lock(&lock[i]);
               omp_set_lock(&lock[r]);
               temp = a[i];
               a[i] = a[r];
               omp_unset_lock(&lock[i]);
               a[r] = temp;
               omp_unset_lock(&lock[r]);
            }
         }
   }        

   for (i = 0; i < size; i++)
      printf("%d \n", a[i]); 
   return 0;
}
