#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int fd = open("example.txt", O_RDONLY | O_CREAT, 0644);
    if (fd == -1) {
        perror("Failed to open file");
        return 1;
    }

    char buffer[128];
    ssize_t bytes_read;

    // Leave 1 byte at the end for the null terminator
    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0'; 
        printf("%s", buffer);
    }

    if (bytes_read == -1) {
        perror("Error reading file");
    }

    close(fd);
    return 0;
}
