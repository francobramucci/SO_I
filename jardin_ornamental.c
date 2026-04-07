#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define N_VISITANTES 20000

int visitantes = 0;

void *molinete(void * arg){
    int i;
    for (i = 0; i < N_VISITANTES; i++){
        visitantes++;
        usleep(200);
    }
}

int main(){
    pthread_t m1,m2;
    pthread_create(&m1, NULL, molinete, NULL);
    pthread_create(&m2, NULL, molinete, NULL);
    pthread_join(m1, NULL);
    pthread_join(m2, NULL);

    printf("Hoy hubo %d visitantes!\n", visitantes);

    return 0;
}
