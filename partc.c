#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("USAGE: partc FILEIN\n");
        return 1;
    }

    int pipefd[2];
    if (pipe(pipefd) < 0) {
        perror("pipe failed");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return 1;
    }


    if (pid > 0) {
        close(pipefd[0]);                    
        dup2(pipefd[1], STDOUT_FILENO);        
        close(pipefd[1]);                      

        char *args[] = { "./parta", argv[1], NULL };
        execv("./parta", args);

        perror("execv parta failed");
        exit(1);
    }

    else {
        close(pipefd[1]);                      
        dup2(pipefd[0], STDIN_FILENO);          
        close(pipefd[0]);                       

        execlp("sort", "sort", "-t,", "-k2", "-n", NULL);

        perror("exec sort failed");
        exit(1);
    }

    return 0;
}
