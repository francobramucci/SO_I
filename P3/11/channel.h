#include <pthread.h>
#include <semaphore.h>

typedef struct {
        int v;
        sem_t ws;
        sem_t rs;
        sem_t rp;
        sem_t wp;
} channel;

void channel_init(channel *c);
void channel_write(channel *c, int v);
int channel_read(channel *c);
