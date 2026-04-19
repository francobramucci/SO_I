#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t a = PTHREAD_MUTEX_INITIALIZER;

void *foo(void *_arg) {
    pthread_mutex_lock(&a);
    printf("Foo!\n");
    sleep(1);
    pthread_mutex_unlock(&a);
    return NULL;
}

int main() {
    pthread_t th;
    pthread_create(&th, NULL, foo, NULL);

    pthread_mutex_t b = a;
    pthread_mutex_lock(&b);
    printf("Bar!\n");
    sleep(1);
    pthread_mutex_unlock(&b);

    pthread_join(th, NULL);
    return 0;
}

/*
 * Al hacer b = a, se copia el estado actual del mutex a, por lo que si a está bloqueado entonces b también lo estará.
 * Sin embargo, al hacer unlock(&a) se desbloquea lo que está almacenado en &a y no afectará a b. Luego, si se ejecuta
 * primero foo antes que b = a entonces el hilo del main quedará por siempre en el lock(&b). En caso contrario, si se
 * hace b = a y luego lock(&b), a no estará bloqueado y foo podrá ejecutarse normalmente
 *
 */
