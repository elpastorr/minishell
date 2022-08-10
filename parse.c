/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 16:53:21 by elpastor          #+#    #+#             */
/*   Updated: 2022/08/10 17:21:25 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		tmp = mod_env(&myenv, name, val, opt);
	// else if (opt == 4)
	return (tmp);
}
