/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 17:14:53 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/19 18:25:17 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		free_env(var);
	}
	else
		util = ft_strdup("Minishell");
	(*j)++;
	return (util);
}

char	*expand_extra(char *tmp, char *util, int *j, int i)
{
	t_env	*var;

	while (ft_isalnum(tmp[*j]) || tmp[*j] == '_')
		(*j)++;
	util = ft_strndup(&tmp[i + 1], (*j - i + 1));
	if (util)
	{
		var = handler(3, NULL, util, NULL);
		free(util);
		if (var)
			return (util = ft_strdup(var->content));
	}
	return (NULL);
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
	if (!tmp)
		return (NULL);
	return (del_unused_quot(tmp));
}

char	*replace_str(char *str, char *new, int j, int i)
{
	char	*tmp;

	tmp = ft_strndup(str, i);
	tmp = ft_strjoin_free(tmp, new, 2);
	tmp = ft_strjoin_free(tmp, &str[j], 0);
	if (!tmp)
	{
		if (new)
			free(new);
		free(str);
		return (NULL);
	}
	if (str)
		free(str);
	return (tmp);
}
