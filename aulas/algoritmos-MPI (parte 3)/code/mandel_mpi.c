#include <stdio.h>
#include <complex.h>
#include <mpi.h>

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
  int i,j,rows,columns,rank,nproc;
  complex z;
  int row[SIZE],hdr;
  unsigned char line[3*SIZE];
  FILE *img;

  MPI_Init(NULL,NULL);
  MPI_Comm_size(MPI_COMM_WORLD,&nproc);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);

  img=fopen("mandel.pam","w");
  MPI_Barrier(MPI_COMM_WORLD);

  if (rank==0) hdr=fprintf(img,"P6\n%d %d 255\n",SIZE,SIZE);

  MPI_Bcast(&hdr,1,MPI_INT,0,MPI_COMM_WORLD);

  xmin=-2; xmax=-1;
  ymin=0; ymax=1;

  for(i=(rank*SIZE)/nproc;i<((rank+1)*SIZE)/nproc;i++){
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

    fseek(img,hdr+3*SIZE*i,SEEK_SET);
    fwrite(line,1,3*SIZE,img);
  }

  MPI_Finalize();
  return 0;
}
