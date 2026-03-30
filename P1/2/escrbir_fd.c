#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int fd = atoi(argv[1]);
    char *buf = argv[2];
    int size = atoi(argv[3]);

    if (write(fd, buf, size) == -1)
        perror("No se pudo escribir (Exec)\n");

    return 0;
}
