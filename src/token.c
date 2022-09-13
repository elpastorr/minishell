/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 16:55:11 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/13 16:52:45 by eleotard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_token(t_token *token)
{
	int		i;
	t_token	*tmp;

	if (token)
	{
		i = 0;
		tmp = token;
		while (tmp)
		{
			printf("token [%d]: TYPE[%d] word=%s\n", i++, tmp->type, tmp->str);
			tmp = tmp->next;
		}
	}
	else
		printf("No token ...\n");
}

t_token	*new_token(t_token *next, char *str, int type)
{
	t_token	*tmp;

	tmp = malloc(sizeof(t_token));
	if (!tmp)
		return (NULL);
	tmp->type = type;
	tmp->str = str;
	tmp->fd = 0; //ajoute pour les redir
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
	ctfree(token, NULL, 't', 2);
	return (NULL);
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
			tmp->type = fout;
			*f_in = 0;
		}
		else if (*f_out == 1)
		{
			tmp->type = fin;
			*f_out = 0;
		}
		else
			tmp->type = word;
	}
}
