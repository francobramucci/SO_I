#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(){
    	
	while(1){
		printf("> ");

        char buffer[1000];
        char* result = fgets(buffer, 1000, stdin);
        if(!result){
            perror("Error leyendo la entrada.\n");
            exit(EXIT_FAILURE);
        }

        if(strcmp(buffer, "exit\n") == 0){
            break;
        }
        
        char* tok = strtok(buffer, " \n\0");
        char* command_name;
        command_name = tok;

        char* argv[22] = {NULL};
        argv[0] = command_name;
        int i = 1;

        while(tok && i <= 21){
            tok = strtok(NULL, " \n\0");
            argv[i] = tok;

            i++; 
        }

        if(i > 21 && !tok){
            printf("Cantidad de argumentos excedida.\n");
            exit(EXIT_FAILURE);
        }

        int pid = fork();

        if(pid == 0){
            int status = execvp(command_name, argv);
            if(status == -1){
                perror("Error al ejecutar el comando.\n");
                exit(EXIT_FAILURE);
            }
        }

        waitpid(pid, NULL, 0);
	}

    return 0;
}
