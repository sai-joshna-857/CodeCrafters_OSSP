#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void run_producer_consumer() {
    int pfd[2];
    pipe(pfd);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    pid_t pid = fork();

    if (pid == 0) {
        close(pfd[1]);
        char buf[4096];
        while (read(pfd[0], buf, sizeof(buf)) > 0);
        close(pfd[0]);
        exit(0);
    } else {
        close(pfd[0]);
        char buf[4096] = {0};
        long total_bytes = 50 * 1024 * 1024;
        long sent = 0;

        while (sent < total_bytes) {
            sent += write(pfd[1], buf, sizeof(buf));
        }
        close(pfd[1]);
        wait(NULL);

        clock_gettime(CLOCK_MONOTONIC, &end);
        double sec = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        double mb = total_bytes / (1024.0 * 1024.0);

        printf("--- Part 1: Pipe Efficiency ---\n");
        printf("Sent: %.2f MB | Time: %.4f s | Speed: %.2f MB/s\n\n", mb, sec, mb / sec);
    }
}

void run_shell_pipeline() {
    printf("--- Part 2: ls -l | grep \".c\" ---\n");
    int pfd[2];
    pipe(pfd);

    if (fork() == 0) {
        dup2(pfd[1], STDOUT_FILENO);
        close(pfd[0]);
        close(pfd[1]);
        char *args[] = {"ls", "-l", NULL};
        execvp("ls", args);
        exit(1);
    }

     if (fork() == 0) {
        dup2(pfd[0], STDIN_FILENO); 
        close(pfd[0]);
        close(pfd[1]);
        char *args[] = {"grep", ".c", NULL};
        execvp("grep", args);
        exit(1);
    }

    close(pfd[0]);
    close(pfd[1]);
    wait(NULL);
    wait(NULL);
}

int main() {
    run_producer_consumer();
    run_shell_pipeline();
    return 0;
}
