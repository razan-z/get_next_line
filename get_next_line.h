#ifndef GET_NEXT_LINE_H
#define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
#endif

size_t ft_strlen(const char *s);
char *find_newline(const char *s);
char *ft_strjoin(char *stash, char *buf);

char *get_next_line(int fd);

#endif