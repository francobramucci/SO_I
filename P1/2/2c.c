#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int copia_salida_estandar = dup(1);

    close(1);
    char *buf = "Hola\n";
    if (write(1, buf, 6) == -1)
        perror("Error al escribir en la salida estandar (1)\n");

    if (write(copia_salida_estandar, buf, 6) == -1)
        perror("Error al escribir en la salida estandar (3)\n");

    return 0;
}

/*
 * Al copiar el file descriptor con dup, observamos que al cerrar una de las
 * copias, podemos seguir escribiendo/leyendo en la otra.
 */
