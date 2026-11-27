#include <stdio.h>
#include <fcntl.h>
#include "get_next_line.h"

int main() {
    int fd = open("example.txt", O_RDONLY | O_CREAT, 0644);
    if (fd == -1) {
        perror("Failed to open file");
        return 1;
    }

    char *line;
    int i = 0;
    while ((line = get_next_line(fd)) != NULL) {
        i++;
        printf("%d line: %s", i,line);
        free(line);
    }

    close(fd);
    return 0;
}
