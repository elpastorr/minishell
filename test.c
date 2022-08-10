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

int	quot_status(char *s, int i)
{
	int		j;
	char	quot;

	j = 0;
	quot = 0;
	while (s[j] && j < i)
	{
		if (s[j] == '"' || s[j] == '\'')
		{
			if (quot == 0)
				quot = s[j];
			else if (quot == s[j])
				quot = 0;
		}
		j++;
	}
	if (quot == '\'')
		return (1);
	if (quot == '"')
		return (2);
	return (0);
}

void	skip_quot(char *s, int *i)
{
	char	quot;

	quot = s[*i];
	(*i)++;
	while (s[*i])
	{
		if (s[*i] == quot)
		{
			(*i)++;
			break ;
		}
		(*i)++;
	}
}

void	get_type(t_token *tmp, int *f_in, int *f_out)
{
	if (!ft_strcmp(tmp->str, ">>"))
		tmp->type = rdout;
	else if (!ft_strcmp(tmp->str, "<<"))
		tmp->type = rdin;
	else if (!ft_strcmp(tmp->str, ">"))
		tmp->type = rout;
	else if (!ft_strcmp(tmp->str, "<"))
		tmp->type = rin;
	else if (!ft_strcmp(tmp->str, "|"))
		tmp->type = pip;
	else
	{
		if (*f_in == 1)
		{
			tmp->type = fin;
			*f_in = 0;
		}
		else if (*f_out == 1)
		{
			tmp->type = fout;
			*f_out = 0;
		}
		else
			tmp->type = word;
	}
}

int	token_syntax(t_token *token)
{
	t_token	*tmp;
	int		ret;

	tmp = token;
	ret = 0;
	if (!token)
		return (NULL);
	if (tmp->type == pip)
		ret = 1;
	while (tmp && !ret)
	{
		if ((tmp->type == pip && !tmp->next) || (tmp->type >= pip && tmp->next && tmp->next->type >= pip))
			ret = 1;
		if (!tmp->next)
			break ;
		tmp = tmp->next;
	}
	if (!ret)
		return (token);
	printf("Minishell: parse error near '%s'\n", tmp->str);
	// exitfree();
	return (NULL);
}

t_token	*new_token(t_token *next, char *str, int type)
{
	t_token	*tmp;

	tmp = malloc(sizeof(t_token));
	if (!tmp)
		return (NULL);
	tmp->type = type;
	tmp->str = str;
	tmp->fd = 1;
	tmp->next = next;
	return (tmp);
}





void	del_unquot_extra(char *s, int *i, int *j, char quot)
{
	int	len;

	if (s[*j] == quot && s[*i] == quot)
	{
		while (s[*j])
		{
			s[*j] = s[*j + 1];
			(*j)++;
		}
		len == --(*i) - 1;
		while (s[*i])
		{
			s[*i] = s[*i + 1];
			(*i)++;
		}
		s[*i - 1] = '\0';
		*i = len;
	}
}

char	*del_unused_quot(char *s)
{
	char	*tmp;
	char	quot;
	int		i;
	int		j;

	i = 0;
	if (!s)
		return (NULL);
	while (s[i])
	{
		if (s[i] == '"' || s[i] == '\'')
		{
			quot = s[i];
			j = i++;
			while (s[i] && s[i] != quot)
				i++;
			del_unquot_extra(s, &i, &j, quot);
		}
		else
			i++;
	}
	tmp = ft_strdup(s);
	if (s)
		free(s);
	return (tmp);
}

t_env	*handler(int opt, char **env, char *name, char *val)
{
	t_env			*tmp;
	static t_env	*myenv;
	static int		exit_status;

	tmp = NULL;
	if (name && !ft_strcmp(name, "?"))
	{
		exit_status = opt;
		free(name);
		return (NULL);
	}
	if (val && !ft_strcmp(val, "?"))
		return (init_env(NULL, NULL, ft_itoa(exit_status)));
	if (opt == 0 && env)
		myenv = init_handler(env, &exit_status);
	// else if (opt == 1)

	// else if (opt == 2)

	else if (opt == 3)
		res = mod_env(&myenv, name, val, opt);

	// else if (opt == 4)
	return (tmp);
}

char	*expand_special(char *tmp, char *util, int *j)
{
	t_env	*var;

	if (tmp[*j] == '$')
		util = ft_itoa(getpid());
	else if (tmp[*j] == '?')
	{
		var = handler(-1, NULL, NULL, "?");
		if (!var)
			return (NULL);
		util = ft_strdup(var->content);
		// free_env(var);
	}
	else
		util = ft_strdup("Minishell");
	(*j)++;
	return (util);
}

char	*expand_extra(char *tmp, char *util, int *j, int i)
{
	t_env	*var;

	while (ft_isalnum(res[*j]) || res[*j] == '_')
		(*j)++;
	util = ft_strndup(&res[i + 1], (*j - i + 1));
	if (util)
	{
		var = handler(3, NULL, util, NULL);
		free(util);
		if (var)
			return (util = ft_strdup(var->content));
	}
	return (NULL);
}

char	*replace_str(char *tmp, char *util, int j, int i)
{
	char	*tmp;
	int		k;
	int		l;
	int		m;

	tmp = ft_newstr(ft_strlen(str) - (i - j) + ft_strlen(new));
	if (!tmp)
	{
		if (new)
			free(new);
		free(str);
		return (NULL);
	}
	k = 0;
	l = 0;
	while (str && str[l] && l < j)
		tmp[k++] = str[l++];
	m = 0;
	while (new && new[m])
		tmp[k++] = new [m++];
	l += i - j;
	ft_strcat(&tmp[k], &str[l]);
	if (str)
		free(str);
	return (tmp);
}

char	*expend_words(char *s, int i)
{
	char	*util;
	char	*tmp;
	int		j;

	tmp = ft_strdup(s);
	if (s)
		free(s);
	if (!tmp)
		return (NULL);
	j = i + 1;
	if (tmp[j] == '?' || tmp[j] == '$' || tmp[j] == '0')
		util = expand_special(tmp, NULL, &j);
	else
		util = expand_extra(tmp, NULL, &j, i);
	tmp = replace_str(tmp, util, j, i);
	if (util)
		free(util);
	if (!tmp)
		return (NULL);
	return (del_unused_quot(tmp));
}

void	tokenize(t_token *token)
{
	t_token	*tmp;
	int		f_in;
	int		f_out;

	tmp = token;
	f_in = 0;
	f_out = 0;
	while (tmp)
	{
		get_type(tmp, f_in, f_out);
		if (tmp->type == rdout || tmp->type == rout || tmp->type == rin)
			f_in = 1;
		if (tmp->type == rdin)
			f_out = 1;
		tmp = tmp->next;
	}
	if (!token_syntax(token))
		return ;
	tokenizer(token);
}

void	tokenizing(t_token *token)
{
	t_token	*tmp;
	int		i;

	tmp = token;
	while (tmp)
	{
		if (tmp->type == word || tmp->type == fout)
		{
			i = 0;
			while (tmp->str && tmp->str[i])
			{
				if (tmp->str[i] == '$' && quot_status(tmp->str, i) != 1 && (ft_isalnum(tmp->str[i + 1]) || tmp->str[i + 1] == '_' || tmp->str[i + 1] == '?' || tmp->str[i + 1] == '$'))
					tmp->str = expend_words(tmmp->str, i);
			}
		}
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
	token = new_token(NULL, strndup(&s[start], i - start), 0);
	while (s[i])
	{
		get_word_index(s, &i, &start);
		if (i > start)
		{
			token = new_token(NULL, strndup(&s[start], i - start), 0);
			printf("|%s|\n", strndup(&s[start], i - start));
		}
	}
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
		add_history(s);
		split_words(s, 0, 0);
		free(s);
	}
}

int main(int ac, char **av)
{
	(void)ac;
	(void)av;

	loop();

	return (0);
}