/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 16:55:11 by elpastor          #+#    #+#             */
/*   Updated: 2022/08/10 17:15:44 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

t_token	*token_syntax(t_token *token)
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
		get_type(tmp, &f_in, &f_out);
		if (tmp->type == rdout || tmp->type == rout || tmp->type == rin)
			f_in = 1;
		if (tmp->type == rdin)
			f_out = 1;
		tmp = tmp->next;
	}
	if (!token_syntax(token))
		return ;
	tokenizing(token);
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
					tmp->str = expend_words(tmp->str, i);
			}
		}
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
