#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define SIZE 1024

int main() {
    char buffer[SIZE];
    int src, dest;
    int bytes;

    src = open("source.txt", O_RDONLY);
    if (src < 0) {
        printf("Cannot open source file!\n");
        return 1;
    }

    dest = open("dest.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest < 0) {
        printf("Cannot create destination file!\n");
        close(src);
        return 1;
    }

    while ((bytes = read(src, buffer, SIZE)) > 0) {
        write(dest, buffer, bytes);
    }

    close(src);
    close(dest);

    printf("File copied successfully!\n");
    return 0;
}
