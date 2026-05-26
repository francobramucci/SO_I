#include "channel.h"

void channel_init(channel *c) {
    sem_init(&c->ws, 0, 1);
    sem_init(&c->rs, 0, 1);
    sem_init(&c->rp, 0, 0);
    sem_init(&c->wp, 0, 0);
}

void channel_write(channel *c, int v) {
    sem_wait(&c->ws);
    sem_wait(&c->rp);

    c->v = v;

    sem_post(&c->wp);
    sem_post(&c->ws);
}

int channel_read(channel *c) {
    sem_wait(&c->rs);
    sem_post(&c->rp);
    sem_wait(&c->wp);

    int v = c->v;

    sem_post(&c->rs);

    return v;
}
