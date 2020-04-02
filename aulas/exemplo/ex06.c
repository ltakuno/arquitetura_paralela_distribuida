#include<omp.h>
#include<stdio.h>
#define MAX 100
int main(){
   double ave=0.0, A[MAX];
   int i;

   for (i = 0; i < MAX; i++)
      A[i] = i;


   #pragma omp parallel for reduction(+:ave)
   for (i = 0; i < MAX; i++)
      ave+=A[i];

   ave = ave / MAX;

   printf("%lf\n", ave);
   return 0;
}
