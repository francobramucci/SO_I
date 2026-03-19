#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main(){
    // Creamos el archivo de 1024 bytes
    
    int fd = creat("./1024", S_IRWXU);
    // La macro S_IRWXU indica el permiso 00700. Es decir que el usuario dueño
    // del archivo tiene permiso de leerlo, escribirlo y ejecutarlo.

    if(fd == -1)
        perror("Error al crear el archivo.\n");

    char buf[1024];
    for(int i = 0; i < 1024; i++){
        buf[i] = (i % 26) + 97;
    }

    if(write(fd, buf, 1024) == -1)
        perror("Error al escribir archivo.\n");

    close(fd);

    fd = open("./1024", O_RDONLY);
    
    char read_buf[27];
    if(read(fd, read_buf, 26) == -1)
        perror("Error al leer el archivo.\n");

    read_buf[26] = 0;

    printf("Se ha leido '%s' del archivo.\n", read_buf);

    return 0;

}
