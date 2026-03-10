#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(){
    int fd[2];
    int cpid;

    if(pipe(fd) == -1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    cpid = fork();
        
    if(cpid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if(cpid == 0){
        close(fd[1]);
        char c;
        while(read(fd[0], &c, 1) > 0)
            printf("%c", c);
        close(fd[0]);
    }

    else{
        close(fd[0]);
        write(fd[1], "hola\n", 5);
        close(fd[1]);
        wait(0);
    }
    
    return 0;
}

