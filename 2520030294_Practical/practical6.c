#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define FIFO_REQ  "/tmp/fifo_req"
#define FIFO_RESP "/tmp/fifo_resp"
volatile sig_atomic_t last_signal = 0;

void signal_handler(int sig) {
    last_signal = sig; 
}

void demo_signals() {
    printf("--- PART 2: POSIX Signals Demo ---\n");

    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);

    printf("Sending SIGUSR1 to self (PID: %d)...\n", getpid());
    kill(getpid(), SIGUSR1);

    if (last_signal == SIGUSR1) {
        printf("Caught SIGUSR1 asynchronously via signal_handler!\n\n");
    }
}
void run_server() {
    int req_fd = open(FIFO_REQ, O_RDONLY);
    int resp_fd = open(FIFO_RESP, O_WRONLY);

    char buf[128];
    for (int i = 0; i < 2; i++) {
        memset(buf, 0, sizeof(buf));
        read(req_fd, buf, sizeof(buf));
        printf("[Server] Received: \"%s\"\n", buf);

        char reply[128];
        snprintf(reply, sizeof(reply), "ACK: %s", buf);
        write(resp_fd, reply, strlen(reply) + 1);
    }

    close(req_fd);
    close(resp_fd);
    exit(0);
}

int main() {
    demo_signals();
    unlink(FIFO_REQ);
    unlink(FIFO_RESP);
    mkfifo(FIFO_REQ, 0666);
    mkfifo(FIFO_RESP, 0666);

    printf("--- PART 1: Named Pipe (FIFO) Multi-Client Demo ---\n");
    if (fork() == 0) {
        run_server();
    }

    if (fork() == 0) {
        run_client(1, "Hello");
    }

    if (fork() == 0) {
        run_client(2, "Ping");
    }

    while (wait(NULL) > 0);

    unlink(FIFO_REQ);
    unlink(FIFO_RESP);

    return 0;
}
