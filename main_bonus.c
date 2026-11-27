#include "get_next_line_bonus.h"
#include <fcntl.h>
#include <stdio.h>

static void	print_and_free(int file_num, char *line)
{
	if (line)
	{
		printf("[File %d]: %s", file_num, line);
		if (line[ft_strlen(line) - 1] != '\n')
			printf("\n");
		free(line);
	}
	else
		printf("[File %d]: (EOF / NULL)\n", file_num);
}

int	main(void)
{
	int		fd1;
	int		fd2;
	int		fd3;
	char	*l1;
	char	*l2;
	char	*l3;

	fd1 = open("file1.txt", O_RDONLY);
	fd2 = open("file2.txt", O_RDONLY);
	fd3 = open("file3.txt", O_RDONLY);
	if (fd1 < 0 || fd2 < 0 || fd3 < 0)
		return (1);
	while (1)
	{
		l1 = get_next_line(fd1);
		l2 = get_next_line(fd2);
		l3 = get_next_line(fd3);
		if (!l1 && !l2 && !l3)
			break ;
		print_and_free(1, l1);
		print_and_free(2, l2);
		print_and_free(3, l3);
		printf("----------------------------------\n");
	}
	close(fd1);
	close(fd2);
	close(fd3);
	return (0);
}
