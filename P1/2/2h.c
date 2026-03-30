#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int fd;

    fd = creat("./archivo.txt", 0700);

    int pid = fork();

    if (pid == 0) {
        char *buf = "Hijo\n";
        // Veamos si en el hijo se puede escribir
        if (write(fd, buf, 5) == -1)
            perror("No se pudo escribir (Hijo)");
    }

    else {
        wait(0);

        char fd_string[10];
        snprintf(fd_string, sizeof(fd_string), "%d", fd);
        // Veamos si el comando del execl puede escribir
        execl("./escribir_fd", "escribir_fd", fd_string, "Exec\n", "5", NULL);
    }

    return 0;
}

/*
 *   Como se puede ver, en ambos casos, los procesos creados por fork y exec
 *   heredan todos los fd abiertos.
 *
 */
