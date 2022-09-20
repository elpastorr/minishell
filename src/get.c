/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 19:20:09 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/20 18:08:39 by eleotard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_name(char *env)
{
	int	i;

	i = 0;
	if (env)
	{
		while (env[i] && env[i] != '=')
			i++;
	}
	if (!env || !env[i])
		return (NULL);
	return (ft_strndup(env, i));
}

char	*get_content(char *env)
{
	while (env != NULL && *env && *env != '=')
		env++;
	if (!env)
		return (NULL);
	env++;
	return (ft_strdup(env));
}

int	get_nbpipe(t_cmd *cmd)
{
	int		i;
	t_cmd	*tmp;

	tmp = cmd;
	i = 0;
	while (tmp->next)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

int	get_exit(void)
{
	t_env	*env;
	int		exit;

	env = handler(-1, NULL, NULL, "?");
	exit = ft_atoi(env->content);
	free_env(env);
	return (exit);
}