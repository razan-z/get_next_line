#include "get_next_line.h"

static char	*ft_free_and_return_null(char *mem)
{
	free(mem);
	return (NULL);
}

// This function reads from the file descriptor into a buffer and appends it to the stash until a newline is found or EOF is reached. It returns the updated stash.
static char	*read_to_stash(int fd, char *stash)
{
	char	*buf;
	ssize_t	bytes_read;

	buf = malloc(((size_t)BUFFER_SIZE + 1) * sizeof(char));
	// buf = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buf)
		return (ft_free_and_return_null(stash));
	bytes_read = 1;
	while (!find_newline(stash) && bytes_read != 0)
	{
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(buf);
			return (ft_free_and_return_null(stash));
		}
		buf[bytes_read] = '\0';
		stash = ft_strjoin(stash, buf);
		if (!stash)
			return (ft_free_and_return_null(buf));
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
		return(ft_free_and_return_null(stash));
	new_stash = (char *)malloc(sizeof(char) * (ft_strlen(stash) - i + 1));
	if (!new_stash)
		return (ft_free_and_return_null(stash));
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
