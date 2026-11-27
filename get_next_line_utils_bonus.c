#include "get_next_line_bonus.h"

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
