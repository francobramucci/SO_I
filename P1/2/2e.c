#include <stdlib.h>
#include <unistd.h>

int main() {
    int pid = fork();

    if (pid == 0) {
        exit(0);
    }

    else {
        sleep(100);
    }

    return 0;
}

/*
 * Observamos con top que el proceso hijo aparece como zombie. Sigue existiendo
 * ya que el padre no ejecuto wait(), por lo tanto no se puede limpiar el
 * proceso.
 */
