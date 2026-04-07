#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define NPUNTOS (int)2e7
#define R (int)1e6

int calcular_puntos_circ(){
    int puntos = 0;
    for(int i = 0; i < NPUNTOS/4; i++){
        int x = rand()%(2*R+1);
        int y = rand()%(2*R+1);
        if((x-R)*(x-R) + (y-R)*(y-R) <= R*R)
            puntos++;
    }
    return puntos;
}

int main(){
    srand(time(NULL));
    pthread_t thread_id;
    
    if (pthread_create(&thread_id, NULL, calcular_puntos_circ, NULL)) {
        return 1;
    }
    if (pthread_create(&thread_id, NULL, calcular_puntos_circ, NULL)) {
        return 1;
    }
    if (pthread_create(&thread_id, NULL, calcular_puntos_circ, NULL)) {
        return 1;
    }

    pthread_join(thread_id, );
    
    int NPuntos = 2e7;
    int r = 100000;
    int puntos_circ = 0;
    for(int i = 0; i < NPuntos; i++){
        int x = rand()%(2*r+1);
        int y = rand()%(2*r+1);
        if((x-r)*(x-r) + (y-r)*(y-r) <= r*r)
            puntos_circ++;
    }

    float pi = (float)(4*puntos_circ)/NPuntos;
    printf("Pi es aproximadamente: %f\n", pi);
}
