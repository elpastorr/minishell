#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

size_t	ft_strlen(const char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strdup(const char *s)
{
	char	*str;
	size_t	i;

	str = (char *)malloc(ft_strlen(s) + 1);
	if (str == NULL)
		return (str);
	i = -1;
	while (s[++i])
		str[i] = s[i];
	str[i] = 0;
	return (str);
}

int	fd_heredoc(char *s)
{
	int		fd;
	char	*file_name;

	fd = -1;
	file_name = ft_strdup("/tmp/heredoc_file0.txt");
	while (fd == -1)
	{
		fd = open(file_name, O_CREAT | O_RDWR, 0644);
		if (fd != -1)
			write(fd, s, ft_strlen(s));
		else
			file_name[17]++;
	}
	// unlink(file_name);
	return (fd);
}

int main(int ac, char **av)
{
    int		fd;
	char	buf[512];
	int		i;
	char	*s;

    fd = fd_heredoc(av[1]);
    printf("%d\n", fd);
	i = read(fd, buf, 512);
	printf("buf : |%s|\n", buf);
	s = ft_strdup(buf);
	printf("|%s|\ni : %d\n", s, i);
}
