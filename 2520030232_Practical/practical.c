#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    char command[50];


    printf("Enter a command (e.g., date, ls, whoami): ");
    scanf("%49s", command);


    pid_t pid = fork();

    if (pid < 0) {
        printf("Fork failed!\n");
        return 1;
    }
    else if (pid == 0) {


        printf("[Child]  My PID is %d (Parent is %d)\n", getpid(), getppid());

        char *args[] = {command, NULL};
        execvp(command, args);


        printf("Command not found!\n");
        exit(1);
    }
    else {


        printf("[Parent] My PID is %d (Spawned Child: %d)\n", getpid(), pid);


        wait(NULL);
        printf("[Parent] Child has finished running.\n");
    }

    return 0;
}
