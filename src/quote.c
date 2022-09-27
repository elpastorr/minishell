/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 17:05:46 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/24 17:03:05 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_next_quot(char *s, int i)
{
	while (s[i] && s[i] != '"' && s[i] != '\'')
		i++;
	return (i);
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
		len = --(*i);
		while (s[*i])
		{
			s[*i] = s[*i + 1];
			(*i)++;
		}
		s[*i - 1] = 0;
		*i = get_next_quot(s, len);
	}
}
