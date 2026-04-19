#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define N_VISITANTES 2

int visitantes = 0;

void *molinete(void *arg) {
    int i;
    for (i = 0; i < N_VISITANTES; i++) {
        int c;
        /* sleep? */
        c = visitantes;
        sleep(1);
        visitantes = c + 1;
        /* sleep? */
    }
    return NULL;
}

int main() {
    pthread_t m1, m2;
    pthread_create(&m1, NULL, molinete, NULL);
    pthread_create(&m2, NULL, molinete, NULL);
    pthread_join(m1, NULL);
    pthread_join(m2, NULL);

    printf("Hoy hubo %d visitantes!\n", visitantes);

    return 0;
}

/*
 * Colocar sleep(1) entre c = visitantes y visitantes = c + 1 provoca que un
 * thread lea el valor de visitantes y espere un segundo, en dicho segundo, el
 * otro thread modifica visitantes y luego el thread original escribe el valor
 * pisando lo que escribió el otro thread.
 */
