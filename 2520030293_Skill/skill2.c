#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#define BUFFER_SIZE 256

void set_raw_mode(struct termios *orig_termios) {
    struct termios raw;
    tcgetattr(STDIN_FILENO, orig_termios);
    raw = *orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void reset_raw_mode(struct termios *orig_termios) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, orig_termios);
}

int main(void) {
    struct termios orig_termios;
    char buffer[BUFFER_SIZE];
    int index = 0;
    char c;

    set_raw_mode(&orig_termios);

    while (1) {
          if (index == 0) {
            printf("\r\nmy_shell> ");
            fflush(stdout);
        }
        if (read(STDIN_FILENO, &c, 1) <= 0) continue;

        if (c == '\n' || c == '\r') {
            buffer[index] = '\0'; 
            printf("\r\n");

            if (strcmp(buffer, "exit") == 0 || strcmp(buffer, "quit") == 0) {
                printf("Exiting interactive loop.\r\n");
                break;
            }
            if (strcmp(buffer, "help") == 0) {
                printf("Commands: 'help', 'status', 'exit'\r\n");
            } else if (strcmp(buffer, "status") == 0) {
                printf("Status: System operational.\r\n");
            } else if (index > 0) {
                printf("Unrecognized command: %s\r\n", buffer);
            }

            index = 0;
            continue;
        }

        if (c == 127 || c == 8) {
            if (index > 0) {
                index--;
                printf("\b \b");
                fflush(stdout);
            }
            continue;
        }

       if (c == 3 || c == 4) {
            printf("\r\nAborted.\r\n");
            break;
        }

        if (c >= 32 && c <= 126) {
            if (index < BUFFER_SIZE - 1) {
                buffer[index++] = c;
                putchar(c);
                fflush(stdout);
            }
        }
    }

    reset_raw_mode(&orig_termios);
    return 0;
}
