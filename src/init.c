/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 19:07:52 by elpastor          #+#    #+#             */
/*   Updated: 2022/08/10 19:16:09 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*init_env(t_env *next, char *name, char *content)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	env->next = next;
	env->name = name;
	env->content = content;
	return (env);
}

static t_env	*init_handler(char **env, int *exit_status)
{
	int		i;
	t_env	*myenv;
	t_env	*tmp;

	*exit_status = 0;
	myenv = init_env(NULL, get_name(env[0]), get_content(env[0]));
	// if (!myenv)
		// exitfree()
	i = 0;
	tmp = myenv;
	while (env[++i])
	{
		tmp->next = init_env(NULL, get_name(env[i]), get_content(env[i]));
		// if (!tmp->next)
			// exitfree();
		tmp = tmp->next;
	}
	return (myenv);
}