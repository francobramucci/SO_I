#include <pthread.h>
#include <unistd.h>

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;

void *p1(void *arg) {
    pthread_mutex_lock(&m1);
    sleep(1);
    pthread_mutex_lock(&m2);

    pthread_mutex_unlock(&m2);
    pthread_mutex_unlock(&m1);

    return NULL;
}

void *p2(void *arg) {
    pthread_mutex_lock(&m2);
    pthread_mutex_lock(&m1);

    pthread_mutex_unlock(&m2);

    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, p1, NULL);
    pthread_create(&t2, NULL, p2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
