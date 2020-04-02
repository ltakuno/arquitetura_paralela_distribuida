#include<omp.h>
#include<stdio.h>
#include<stdlib.h>

#define NBUCKETS 10000

int main(){
   omp_lock_t lck;
   long int hist[NBUCKETS];
   int ival, i;


   #pragma omp parallel for 
   for (i = 0; i < NBUCKETS; i++){
      hist[i] = 0; 
   }

   omp_init_lock(&lck);

   #pragma omp parallel for
   for (i = 0; i < NBUCKETS; i++) {
      ival = rand() % NBUCKETS;
      omp_set_lock(&lck);
      hist[ival]++;
      omp_unset_lock(&lck);
   }

   omp_destroy_lock(&lck);
   for (i = 0; i < NBUCKETS; i++) {
      printf("hist[%d] = %ld \n", i, hist[i]);
   }

   return 0;
}
