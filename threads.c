#include <pthread.h>
#include <stdio.h>

static void* print_hello(void *arg){
    printf("Hello world\n");
}

int main(){
    pthread_t thread_id;
    printf("Sin inicializar thread_id: %ld\n", thread_id);
    
    if (pthread_create(&thread_id, NULL, print_hello, NULL)) {
        return 1;
    }
    printf("Inicializado thread_id: %ld\n", thread_id);

    pthread_join(thread_id, NULL);
    
    return 0;
}
