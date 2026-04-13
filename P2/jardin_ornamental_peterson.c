#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#define N_VISITANTES 20000000

int visitantes = 0;
int flags[2] = {0};
int turno = 0;

void *molinete(void *arg) {
    int m_id = *((int *)arg);
    int otro = (m_id + 1) % 2;

    int i;

    for (i = 0; i < N_VISITANTES; i++) {
        flags[m_id] = 1;
        turno = otro;
        while (turno == otro && flags[otro] == 1)
            ;
        visitantes++;

        flags[m_id] = 0;
    }

    return NULL;
}

int main() {
    int m1_id = 0;
    int m2_id = 1;
    pthread_t m1, m2;
    pthread_create(&m1, NULL, molinete, &m1_id);
    pthread_create(&m2, NULL, molinete, &m2_id);
    pthread_join(m1, NULL);
    pthread_join(m2, NULL);

    printf("Hoy hubo %d visitantes!\n", visitantes);

    return 0;
}
