#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define N_VISITANTES 2000000

static inline void incl(int *p) {
    asm("lock; incl %0" : "+m"(*p) : : "memory");
}

int visitantes = 0;

void *molinete(void *arg) {
    int i;
    for (i = 0; i < N_VISITANTES; i++) {
        incl(&visitantes);
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
