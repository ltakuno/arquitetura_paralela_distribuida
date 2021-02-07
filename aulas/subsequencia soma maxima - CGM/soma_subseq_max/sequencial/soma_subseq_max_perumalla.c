#include<stdio.h>
#include<limits.h>

#define n 16
#define max(a, b) ((a) > (b)) ? (a) : (b)
#define min(a, b) ((a) < (b)) ? (a) : (b)

int main(int argc, char *argv[]) {
   int q[n] = {3,2,-7,11,10,-6,4,9,-6,1,-2,-3,4,-3,0,2}; // exemplo fixo
   int psum[n] = {0};
   int ssum[n] = {0};
   int pmax[n] = {0};
   int smax[n] = {0};

   // calcular a soma de prefixos PSUM
   psum[0] = q[0];
   for (int i = 1; i < n; i++) {
      psum[i] = q[i] + psum[i-1];
   }


   // calcular a soma de sufixos SSUM
   ssum[n-1] = q[n-1];
   for (int i = n - 2; i >= 0; i--){
      ssum[i] = q[i] + ssum[i+1];   	   
   } 

   // calcular o sufixo máximo de PSUM no array SMAX
   smax[n - 1] = psum[n - 1];
   for (int i = n - 2; i >= 0; i--){
      smax[i]  = max(psum[i], smax[i + 1]);
   }

   // calcular o prefixo máximo de SSUM no array PMAX
   pmax[0] = ssum[0];
   for (int i = 1; i < n; i++){
      pmax[i] = max(ssum[i], pmax[i - 1]);
   }

   int m[n];
   int ms[n];
   int mp[n];

   for (int i = 0; i < n; i ++) {
      ms[i] = pmax[i] - ssum[i] + q[i];
      mp[i] = smax[i] - psum[i] + q[i];
      m[i] = ms[i] + mp[i] - q[i];
      printf("%d ", m[i]);
   }
   printf("\n");


   // encontra a maior soma
   int max_sum = m[0];
   for (int i = 1; i < n; i++) { 
      if (m[i] > max_sum) { 
         max_sum = m[i];
      }
   }
  
   
   int binary[n]; 
   int segment[n];

   segment[0] = 1;
   for (int i = 0; i < n; i++){
      binary[i] = m[i] == max_sum ? -1 : 0;
      if (i != 0 && binary[i] == binary[i-1])
         segment[i] = segment[i-1] + 1;
   }

   int max_seg = INT_MIN;
   int min_seg = INT_MAX;
   int total = 0;
   int count = 0;
  
   printf("min = %d, max = %d\n ", min_seg, max_seg);


   int k = -1;
   for (int i = 0; i < n; i++, k++) {
      segment[i] = segment[i] & binary[i];
      printf("segment[%d] = %d\n", i, segment[i]);

      if (k >= 0 && segment[k] > 0){
         total++;
	 max_seg = max(segment[k], max_seg);
         min_seg = min(segment[k], min_seg); 
      }
   }
   
   printf("maxsum = %d, maior = %d, menor = %d, total = %d\n", max_sum, max_seg, min_seg, total);
   return 0;
}
