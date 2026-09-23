#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t c1, c2;

    c1 = fork();
    if (c1 == 0) {
        printf("[Child 1] PID: %d, PPID: %d - Running for 2 seconds...\n", getpid(), getppid());
        sleep(2);
        printf("[Child 1] Exiting.\n");
        exit(0);
    }

    c2 = fork();
    if (c2 == 0) {
        printf("[Child 2] PID: %d, PPID: %d - Exiting immediately (Becomes Zombie)...\n", getpid(), getppid());
        exit(0); 
    }
    printf("[Parent] PID: %d created Child 1 (%d) and Child 2 (%d)\n", getpid(), c1, c2);

    printf("[Parent] Sleeping 10 seconds. Check terminal now: 'ps -l' shows Child 2 as 'Z' (Zombie)!\n");
    sleep(10);

    printf("\n[Parent] Fixing Child 1 using waitpid()...\n");
    waitpid(c1, NULL, 0);
    printf("[Parent] Child 1 cleared.\n");

    printf("[Parent] Fixing Zombie Child 2 using wait()...\n");
    wait(NULL);
    printf("[Parent] Child 2 zombie eliminated! No zombies left.\n");

    return 0;
}
