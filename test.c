#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	if (n == 0)
		return (0);
	i = 0;
	while (i < n - 1 && s1[i] == s2[i] && s1[i])
		i++;
	return ((const unsigned char)s1[i] - (const unsigned char)s2[i]);
}

char	*ft_strndup(const char *s, int n)
{
	char	*str;
	int		i;

	str = (char *)malloc(n + 1);
	if (str == NULL)
		return (0);
	i = -1;
	while (s[++i] && i < n)
		str[i] = s[i];
	str[i] = 0;
	return (str);
}

void	skip_cot(char *s, int *i)
{
	char	cot;

	cot = s[*i];
	(*i)++;
	while (s[*i])
	{
		if (s[*i] == cot)
		{
			(*i)++;
			break ;
		}
		(*i)++;
	}
}

void	get_word_index(char *s, int *i, int *start)
{
	while (s[*i] == ' ' || s[*i] == '\t')
		(*i)++;
	*start = *i;
	if (!ft_strncmp(&s[*i], "<<", 2) || !ft_strncmp(&s[*i], ">>", 2))
		(*i) += 2;
	else if (s[*i] == '<' || s[*i] == '>' || s[*i] == '|')
		(*i)++;
	else
	{
		while (s[*i] && s[*i] != '|' && s[*i] != '<' && s[*i] != '>' && s[*i] != ' ' && s[*i] != '\t')
		{
			if (s[*i] == '"' || s[*i] == '\'')
				skip_cot(s, i);
			else
				(*i)++;
		}
	}
}

void	split_words(char *s, int i, int start)
{
	t_token	*token;
	t_token	*tmp;
	char	*str;

	get_word_index(s, &i, &start);
	printf("|%s|\n", strndup(&s[start], i - start));
	while (s[i])
	{
		get_word_index(s, &i, &start);
		if (i > start)
		{
			printf("|%s|\n", strndup(&s[start], i - start));
		}
	}
	// token = new_token(NULL, strndup(&s[start], i - start), 0);
}

void    loop(void)
{
	char	*s;

	s = NULL;
	while (1)
	{
		s = readline("\e[1m\e[31m\002""Minishell : ""\001\e[0m\002");
		if (s == NULL)
			return ;
		// add_history(s); //??
		split_words(s, 0, 0);
	}
}

int main(int ac, char **av)
{
	(void)ac;
	(void)av;

	loop();

	return (0);
}