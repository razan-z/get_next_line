#include "get_next_line.h"

size_t ft_strlen(const char *s)
{
    int i;

    if(!s)
        return(0);
    i = 0;
    while(s[i])
        i++;
    return(i);
}
// This function searches for a newline character in the given string and returns a pointer to it if found, or NULL if not found.
char *find_newline(const char *s)
{
    int i;

    if(!s)
        return (NULL);
    i = 0;
    while(s[i])
    {
        if(s[i] == '\n')
            return ((char *)&s[i]);
        i++;
    }
    return (NULL);
}

// This function concatenates two strings, stash and buf, into a new string. It allocates memory for the new string, copies the contents of stash and buf into it, and frees the old stash. It returns the new concatenated string.
// If you do not free(stash) inside ft_strjoin, the pointer to the old stash is lost forever, causing a memory leak on every single read() iteration.
char *ft_strjoin(char *stash, char *buf)
{
    char *str;
    size_t len1;
    size_t len2;
    size_t i;
    size_t j;

    if(!stash && !buf)
        return (NULL);
    len1 = ft_strlen(stash);
    len2 = ft_strlen(buf);
    str = (char *)malloc(len1 + len2 + 1);
    if(!str)
        return (free(stash), NULL);
    i = 0;
    while(i < len1)
    {
        str[i] = stash[i];
        i++;
    }
    j = 0;
    while(j < len2)
        str[i++] = buf[j++];
    str[i] = '\0';
    free(stash);
    return (str);
}
