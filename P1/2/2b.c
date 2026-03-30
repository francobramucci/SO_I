#include <stdio.h>
#include <unistd.h>

int main() {

    close(0);
    char read_buf[6];
    if (read(0, read_buf, 6) == -1)
        perror("Error al leer en la entrada estandar\n");

    printf("Leo %s\n", read_buf);

    return 0;
}

/*
 * Observamos que al cerrar la entrada estándar y luego leer en
 * ella, la función read retorna -1 y con perror se imprime el error "Bad file
 * descriptor".
 */
