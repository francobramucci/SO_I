#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    int fd;

    fd = shm_open("/mem_padre_hijo", O_CREAT | O_RDWR, 0700);
    ftruncate(fd, 4096);

    // El primer argumento es la dirección de memoria donde comenzará el mapeo. Al pasar NULL como argumento, el kernel
    // elige la dirección.

    // El segundo argumento es el largo de la memoria a mapear

    // El tercer argumento refiere a las protecciones de memoria

    // El cuarto refiere a cómo se escriben los datos. MAP_SHARED indica que los cambios son visibles para otros
    // procesos que mapeen la misma memoria. MAP_PRIVATE indica que al querer escribir en la memoria compartida se
    // utiliza COW, es decir que se hace una copia privada de esa página y el resto de procesos no se enteran del
    // cambio. MAP_FIXED indica que se mapeara la memoria exactamente en addr (problemática ya que puede pisar datos
    // importantes o incluso el mismo código).

    // El quinto es el fd.

    // El sexto es el offset, que suele ser 0 cuando se refiere a porciones de memoria. Si se utilizara un archivo en
    // disco podría modificarse para indicar una parte específica del archivo y no cargarlo entero en memoria.

    char *pa = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    int pid = fork();

    // Esto presenta problemas por la condición de carrera.
    // Se solucionará más adelante con semáforo
    if (pid == 0) {
        printf("Mi padre escribio %c en pa[0] y %c en pa[1]", pa[0], pa[1]);
    } else {
        pa[0] = 'a';
        pa[1] = 'b';
    }

    munmap(pa, 4096);
    shm_unlink("/mem_padre_hijo");
    return 0;
}
