///  https://www.mjr19.org.uk/courses/MPI/

#include <stdio.h>
#include<complex.h>

#define SIZE 800

int mandel(complex z0){
  int i;
  complex z;

  z=z0;
  for(i=1;i<320;i++){
    z=z*z+z0;
    if ((creal(z)*creal(z))+(cimag(z)*cimag(z))>4.0) break;
  }

  return i;
}


int main(){
  double xmin,xmax,ymin,ymax;
  int i,j,rows,columns;
  complex z;
  int row[SIZE];
  unsigned char line[3*SIZE];
  FILE *img;

  img=fopen("mandel.pam","w");
  fprintf(img,"P6\n%d %d 255\n",SIZE,SIZE);

  xmin=-2; xmax=1;
  ymin=-1.5; ymax=1.5;

  for(i=0;i<SIZE;i++){
    for(j=0;j<SIZE;j++){
      z=xmin+j*((xmax-xmin)/SIZE)+(ymax-i*((ymax-ymin)/SIZE))*I;
      row[j]=mandel(z);
    }

    for(j=0;j<SIZE;j++){
      if (row[j]<=63){
        line[3*j]=255;
        line[3*j+1]=line[3*j+2]=255-4*row[j];
      }
      else{
        line[3*j]=255;
        line[3*j+1]=row[j]-63;
        line[3*j+2]=0;
      }
      if (row[j]==320) line[3*j]=line[3*j+1]=line[3*j+2]=255;
    }

    fwrite(line,1,3*SIZE,img);
  }

  fclose(img);
  return 0;
}


