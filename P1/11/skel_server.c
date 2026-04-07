#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

/*
 * Para probar, usar netcat. Ej:
 *
 * $ nc localhost 4040
 * NUEVO
 * 0
 * NUEVO
 * 1
 * CHAU
 */

void handler(int s) {
    shm_unlink("/shm");
    exit(0);
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

void handle_conn(int csock, int *pa) {
    char buf[200];
    int rc;

    while (1) {
        /* Atendemos pedidos, uno por linea */
        rc = fd_readline(csock, buf);
        if (rc < 0)
            quit("read... raro");

        if (rc == 0) {
            /* linea vacia, se cerró la conexión */
            close(csock);
            return;
        }

        if (!strcmp(buf, "NUEVO")) {
            char reply[20];
            sprintf(reply, "%d\n", *pa);
            (*pa)++;
            write(csock, reply, strlen(reply));
        } else if (!strcmp(buf, "CHAU")) {
            close(csock);
            return;
        }
    }
}

void wait_for_clients(int lsock, int *pa) {
    int csock;

    /* Esperamos una conexión, no nos interesa de donde viene */
    csock = accept(lsock, NULL, NULL);
    if (csock < 0)
        quit("accept");

    int cpid = fork();

    if (cpid == 0) {
        /* Atendemos al cliente */
        close(lsock);
        handle_conn(csock, pa);
        exit(0);
    }

    else {
        /* Volvemos a esperar conexiones */
        close(csock);
        wait_for_clients(lsock, pa);
    }
}

/* Crea un socket de escucha en puerto 4040 TCP */
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
    sa.sin_port = htons(4040);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);

    /* Bindear al puerto 4040 TCP, en todas las direcciones disponibles */
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

    signal(SIGINT, handler);

    // Esta linea hace que el kernel elimine automáticamente al hijo y libere su PID. Evita zombies.
    signal(SIGCHLD, SIG_IGN);

    int fd = shm_open("/shm", O_CREAT | O_RDWR, 0700);
    ftruncate(fd, 4);

    int *pa = mmap(NULL, 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    int lsock;
    lsock = mk_lsock();
    wait_for_clients(lsock, pa);
}
