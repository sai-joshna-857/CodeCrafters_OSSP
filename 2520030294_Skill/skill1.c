#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    printf("[Parent] Initial PID: %d, PPID: %d\n", getpid(), getppid());

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return EXIT_FAILURE;
    } 
    else if (pid == 0) {
        printf("[Child]  My PID: %d | Parent PID: %d\n", getpid(), getppid());
        printf("[Child]  Pausing 5 seconds to allow tree inspection...\n");
        sleep(5);

        printf("[Child]  Executing 'ls -l' via execvp...\n");
        char *args[] = {"ls", "-l", NULL};
        execvp(args[0], args);

        perror("execvp failed");
        exit(EXIT_FAILURE);
    } 
    else {
        printf("[Parent] Spawned Child with PID: %d\n", pid);
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("[Parent] Child terminated with exit code: %d\n", WEXITSTATUS(status));
        }
    }

    return EXIT_SUCCESS;
}
EOF
