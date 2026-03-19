#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){

    if(argc < 4){
        printf("Ingresar path del ejecutable, nombre y cantidad de segundos.\n");
        exit(EXIT_FAILURE);
    }

    char* path = argv[1];
    char* nombre = argv[2];
    unsigned time = (unsigned)atoi(argv[3]);

    while(1){
        int pid = fork();
        if(pid == -1){
            perror("Error al ejecutar fork");
            exit(EXIT_FAILURE);
        }

        if(pid == 0){
            execl(path, nombre, (char*)NULL);
            exit(EXIT_SUCCESS); // En caso que el execl fracase.
        }
          
        waitpid(pid, NULL, 0);

        sleep(time);
    }

    return 0;
}
