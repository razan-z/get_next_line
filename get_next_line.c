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

// This function reads from the file descriptor into a buffer and appends it to the stash until a newline is found or EOF is reached. It returns the updated stash.
static char	*read_to_stash(int fd, char *stash)
{
	char	*buf;
	ssize_t	bytes_read;

	buf = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buf)
		return (free(stash), NULL);
	bytes_read = 1;
	while (!find_newline(stash) && bytes_read != 0)
	{
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(buf);
			free(stash);
			return (NULL);
		}
		buf[bytes_read] = '\0';
		stash = ft_strjoin(stash, buf);
		if (!stash)
			return (free(buf), NULL);
	}
	free(buf);
	return (stash);
}

// This function extracts a line from the stash, including the newline character if present, and returns it as a new string. It allocates memory for the new line and returns NULL if the stash is empty.
static char	*extract_line(char *stash)
{
	int		i;
	char	*line;

	i = 0;
	if (!stash[i])
		return (NULL);
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		i++;
	line = (char *)malloc(sizeof(char) * (i + 1));
	if (!line)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
	{
		line[i] = stash[i];
		i++;
	}
	if (stash[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

// This function cleans the stash by removing the extracted line and keeping the remaining content. It allocates memory for the new stash and frees the old one. If there is no remaining content, it returns NULL.
static char	*save_stash_leftovers(char *stash)
{
	int		i;
	int		j;
	char	*new_stash;

	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (!stash[i])
	{
		free(stash);
		return (NULL);
	}
	new_stash = (char *)malloc(sizeof(char) * (ft_strlen(stash) - i + 1));
	if (!new_stash)
		return (free(stash), NULL);
	i++;
	j = 0;
	while (stash[i])
		new_stash[j++] = stash[i++];
	new_stash[j] = '\0';
	free(stash);
	return (new_stash);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*stash;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = read_to_stash(fd, stash);
	if (!stash)
		return (NULL);
	line = extract_line(stash);
	if (!line)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	stash = save_stash_leftovers(stash);
	return (line);
}