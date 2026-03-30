#include <stdio.h>
#include <unistd.h>

int main() {

    close(1);
    char *buf = "Hola\n";
    if (write(1, buf, 6) == -1)
        perror("Error al escribir en la salida estandar\n");

    return 0;
}

/*
 * Observamos que al cerrar la salida estándar y luego intentar escribir en
 * ella, la función write retorna -1 y con perror se imprime el error "Bad file
 * descriptor".
 */
