#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_ELEMENTOS 10000
#define MAX_EVENTOS 1000

/*
 * Para probar, usar netcat. Ej:
 *
 * $ nc localhost 3942
 */

typedef struct {
        char *k;
        char *v;
} kv;

typedef struct {
        kv *arr;
        int tam;
        int ult;
} _vec;

typedef _vec *vect;
vect map;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

vect inicializar(int s) {
    vect vec = malloc(sizeof(_vec));
    vec->arr = malloc(s * sizeof(kv));
    vec->tam = s;
    vec->ult = -1;
    return vec;
}

void insertar(vect vec, char *key, char *val) {
    for (int i = 0; i <= vec->ult; i++) {
        if (strcmp(key, vec->arr[i].k) == 0) {
            free(vec->arr[i].v);
            vec->arr[i].v = strdup(val);
            return;
        }
    }

    if (vec->ult + 1 == vec->tam) {
        vec->tam *= 2;
        vec->arr = realloc(vec->arr, sizeof(kv) * vec->tam);
    }

    vec->ult++;
    vec->arr[vec->ult].k = strdup(key);
    vec->arr[vec->ult].v = strdup(val);
}

char *buscar(vect vec, char *key) {
    for (int i = 0; i <= vec->ult; i++) {
        if (strcmp(key, vec->arr[i].k) == 0)
            return vec->arr[i].v;
    }
    return NULL;
}

void eliminar(vect vec, char *key) {
    for (int i = 0; i <= vec->ult; i++) {
        if (!strcmp(key, vec->arr[i].k)) {
            free(vec->arr[i].v);
            vec->arr[i].v = NULL;
        }
    }
}

void quit(char *s) {
    perror(s);
    abort();
}

int fd_readline(int fd, char *buf) {
    int rc;
    int i = 0;

    /*
     * Leemos de a un caracter (no muy eficiente...) hasta
     * completar una línea.
     */
    while ((rc = read(fd, buf + i, 1)) > 0) {
        if (buf[i] == '\n')
            break;
        i++;
    }

    if (rc < 0)
        return rc;

    buf[i] = 0;
    return i;
}

void *handle_conn(void *arg) {
    int csock = *((int *)arg);
    free(arg);
    char buf[200];
    char reply[200];
    int rc;

    char *ptr = NULL;

    while (1) {
        /* Atendemos pedidos, uno por linea */
        rc = fd_readline(csock, buf);
        char *comando = strtok_r(buf, " \n\0", &ptr);

        if (!strcmp(comando, "PUT")) {
            char *key = strtok_r(NULL, " \n\0", &ptr);
            char *value = strtok_r(NULL, " \n\0", &ptr);
            pthread_mutex_lock(&mutex);
            insertar(map, key, value);
            pthread_mutex_unlock(&mutex);

            write(csock, "OK\n", 3);
        }

        else if (!strcmp(comando, "GET")) {
            char *val;
            char *key = strtok_r(NULL, " \n\0", &ptr);
            pthread_mutex_lock(&mutex);
            val = buscar(map, key);
            pthread_mutex_unlock(&mutex);
            if (val) {
                sprintf(reply, "OK %s\n", val);
                write(csock, reply, strlen(reply));
            } else
                write(csock, "NOTFOUND\n", 9);
        }

        else if (!strcmp(comando, "DEL")) {
            char *key = strtok_r(NULL, " \n\0", &ptr);
            pthread_mutex_lock(&mutex);
            eliminar(map, key);
            pthread_mutex_unlock(&mutex);
            write(csock, "OK\n", 3);
        }

        else {
            write(csock, "EINVAL\n", 7);
        }

        if (rc < 0)
            quit("read... raro");

        if (rc == 0) {
            /* linea vacia, se cerró la conexión */
            close(csock);
            return NULL;
        }
    }
}

void wait_for_clients(int lsock) {
    int *csock = malloc(sizeof(int));

    /* Esperamos una conexión, no nos interesa de donde viene */
    *csock = accept(lsock, NULL, NULL);
    if (*csock < 0)
        quit("accept");

    /* Atendemos al cliente */
    pthread_t id;
    pthread_create(&id, NULL, handle_conn, csock);
    pthread_detach(id);

    /* Volvemos a esperar conexiones */
    // close(csock);
    wait_for_clients(lsock);
}

/* Crea un socket de escucha en puerto 3942 TCP */
int mk_lsock() {
    struct sockaddr_in sa;
    int lsock;
    int rc;
    int yes = 1;

    /* Crear socket */
    lsock = socket(AF_INET, SOCK_STREAM, 0);
    if (lsock < 0)
        quit("socket");

    /* Setear opción reuseaddr... normalmente no es necesario */
    if (setsockopt(lsock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == 1)
        quit("setsockopt");

    sa.sin_family = AF_INET;
    sa.sin_port = htons(3942);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);

    /* Bindear al puerto 3942 TCP, en todas las direcciones disponibles */
    rc = bind(lsock, (struct sockaddr *)&sa, sizeof sa);
    if (rc < 0)
        quit("bind");

    /* Setear en modo escucha */
    rc = listen(lsock, 10);
    if (rc < 0)
        quit("listen");

    return lsock;
}

int main() {
    map = inicializar(MAX_ELEMENTOS);
    int listen_sock;
    listen_sock = mk_lsock();

    struct epoll_event ev, events[MAX_EVENTOS];

    int epollfd = epoll_create1(0);
    if (epollfd == -1) {
        quit("epoll_create1");
    }

    ev.events = EPOLLIN;
    ev.data.fd = listen_sock;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
        perror("epoll_ctl: listen_sock");
        exit(EXIT_FAILURE);
    }

    for (int nfds, n, conn_sock;;) {
        nfds = epoll_wait(epollfd, events, MAX_EVENTOS, -1);
        if (nfds == -1)
            quit("epoll_wait");

        for (n = 0; n < nfds; ++n) {
            if (events[n].data.fd == listen_sock) {
                conn_sock = accept(listen_sock, NULL, NULL);
                if (conn_sock == -1)
                    quit("accept");
            }
        }
    }
}
