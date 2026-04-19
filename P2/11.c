#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

/*
 * Variante lockfree: ~0.35s
 * Variante lock CAS: ~6.3s
 */

#define N_VISITANTES 4000000
#define N_HILOS 100

int visitantes = 0;
int lock = 0;

static inline char CAS(volatile int *ptr, int expected, int new_value) {
    char success;
    // Importante el lock para que la instrucción sea atómica en contextos
    // multinúcleos
    asm("lock cmpxchgl %3, %1 \n\t" // El hardware intenta el intercambio
        "sete %0"                   // Capturamos el resultado (Zero Flag)

        : "=q"(success), // %0: Salida (bool), en un registro de 1 byte
          "+m"(*ptr),    // %1: La memoria compartida (Lectura/Escritura)
          "+a"(expected) // %2: Fuerza a poner 'expected' en el registro EAX

        : "r"(new_value) // %3: El nuevo valor, en cualquier registro libre

        : "memory", "cc" // Clobbers: Avisamos que tocamos RAM y Flags
    );

    return success;
}

void *molinete_lockfree(void *arg) {
    int expected;
    int new_value;
    for (int i = 0; i < N_VISITANTES / N_HILOS; i++) {
        do {
            expected = visitantes;
            new_value = expected + 1;
        } while (!CAS(&visitantes, expected, new_value));
    }
    return NULL;
}

void *molinete_lock_CAS(void *arg) {
    for (int i = 0; i < N_VISITANTES / N_HILOS; i++) {
        while (!CAS(&lock, 0, 1))
            ;
        visitantes++;
        lock = 0;
    }

    return NULL;
}

int main() {
    pthread_t id[N_HILOS];
    for (int i = 0; i < N_HILOS; i++) {
        pthread_create(&id[i], NULL, molinete_lock_CAS, NULL);
    }

    for (int i = 0; i < N_HILOS; i++) {
        pthread_join(id[i], NULL);
    }

    printf("Hoy hubo %d visitantes!\n", visitantes);

    return 0;
}
