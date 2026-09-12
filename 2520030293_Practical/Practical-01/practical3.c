#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        printf("[Child] PID: %d exiting.\n", getpid());
        exit(0);
    } else {
        printf("[Parent] PID: %d waiting for child...\n", getpid());

        wait(NULL);
        printf("[Parent] Child cleaned up. No zombie created.\n");
    }
    return 0;
}
