#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thr_01(void *param){
    int id  = *((int *)param);
    printf("%d (%ld)\n", id, pthread_self());
}

int main()
{
    int i;
    pthread_t tid[10];
    int id[10];
    
    for (i = 0; i < 10; i++)
       id[i] = i;

    for (i = 0; i < 10; i++)
       pthread_create(&tid[i], NULL, thr_01, &id[i]);    


    for (i = 0; i < 10; i++)
       pthread_join(tid[i], NULL);
    return 0;
}

