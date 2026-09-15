#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

typedef struct Node { char *s; struct Node *p, *n; } Node;
static struct termios orig;

void raw(int on) {
    if (on) {
        tcgetattr(0, &orig);
        struct termios r = orig;
        r.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(0, TCSAFLUSH, &r);
    } else tcsetattr(0, TCSAFLUSH, &orig);
}

int main(void) {
    raw(1);
    size_t cap = 16, len = 0;
    char *buf = malloc(cap), c;
    buf[0] = 0;
    Node *head = NULL, *tail = NULL, *cur = NULL;

    printf("my_shell> "); fflush(stdout);

    while (read(0, &c, 1) > 0) {
        if (c == '\033') {
            char s[2];
            if (read(0, &s[0], 1) && read(0, &s[1], 1) && s[0] == '[') {
                if (s[1] == 'A' && (cur ? cur->p : tail)) cur = cur ? cur->p : tail;
                else if (s[1] == 'B' && cur) cur = cur->n;
                else continue;
                len = cur ? strlen(cur->s) : 0;
                while (len + 1 > cap) buf = realloc(buf, cap *= 2);
                strcpy(buf, cur ? cur->s : "");
                printf("\r\033[Kmy_shell> %s", buf); fflush(stdout);
            }
        } else if (c == '\n' || c == '\r') {
            printf("\r\n");
            if (!strcmp(buf, "exit")) break;
            if (len) {
                Node *m = malloc(sizeof(Node));
                m->s = strdup(buf); m->n = NULL; m->p = tail;
                if (tail) tail->n = m; else head = m;
                tail = m;
                printf("Executed: %s\r\n", buf);
            }
            len = 0; buf[0] = 0; cur = NULL;
            printf("my_shell> "); fflush(stdout);
        } else if ((c == 127 || c == 8) && len) {
            buf[--len] = 0;
            printf("\b \b"); fflush(stdout);
        } else if (c >= 32 && c <= 126) {
            if (len + 2 >= cap) buf = realloc(buf, cap *= 2);
            buf[len++] = c; buf[len] = 0;
            putchar(c); fflush(stdout);
        }
    }

    free(buf);
    while (head) { Node *nxt = head->n; free(head->s); free(head); head = nxt; }
    raw(0);
    return 0;
}
