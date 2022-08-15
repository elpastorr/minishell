/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 15:33:29 by elpastor          #+#    #+#             */
/*   Updated: 2022/08/15 16:25:02 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*add_env(t_env *env, char *name, char *content)
{
	t_env	*tmp;
	
	tmp = env;
	if (!name)
		return (env);
	while (tmp)
		tmp = tmp->next;
	tmp = init_env(NULL, ft_strdup(name), ft_strdup(content));
	if (!tmp)
		exit_free(NULL, "Error add_env", 0, 1);
	return (tmp);
}

static t_env	*del_env(t_env **env, char *del, t_env *save, t_env *tmp)
{
	if (*env && del && (!ft_strncmp((*env)->name, del, ft_strlen(del))))
	{
		*env = (*env)->next;
		tmp->next = NULL;
		free_env(tmp);
		return (*env);
	}
	while (tmp)
	{
		if (del && !ft_strncmp(tmp->name, del, ft_strlen(del)))
		{
			save->next = tmp->next;
			tmp->next = NULL;
			free_env(tmp);
			tmp = save;
			break ;
		}
		save = tmp;
		tmp = tmp->next;
	}
	if (!tmp)
		save = NULL;
	return (save);
}

static t_env	*init_handler(char **env, int *exit_status)
{
	int		i;
	t_env	*myenv;
	t_env	*tmp;

	*exit_status = 0;
	myenv = init_env(NULL, get_name(env[0]), get_content(env[0]));
	if (!myenv)
		exit_free(NULL, "Error init_handler", 0, 1);
	i = 0;
	tmp = myenv;
	while (env[++i])
	{
		tmp->next = init_env(NULL, get_name(env[i]), get_content(env[i]));
		if (!tmp->next)
			exit_free(NULL, "Error init_handler", 0, 1);
		tmp = tmp->next;
	}
	return (myenv);
}

static t_env	*mod_env(t_env **env, char *name, char *content)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp && name)
	{
		if (!ft_strcmp(name, tmp->name))
		{
			if (content)
			{
				free(tmp->content);
				tmp->content = ft_strdup(content);
			}
			return (tmp);
		}
		if (!tmp->next)
			if (name && content)
				return (tmp->next = init_env(NULL, ft_strdup(name), ft_strdup(content)));
		tmp = tmp->next;
	}
	if (!tmp && name && content)
		return (*env = init_env(NULL, ft_strdup(name), ft_strdup(content)));
	return (tmp);
}

t_env	*handler(int opt, char **env, char *name, char *content)
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
	if (content && !ft_strcmp(content, "?"))
		return (init_env(NULL, NULL, ft_itoa(exit_status)));
	if (opt == 0 && env)
		myenv = init_handler(env, &exit_status);
	else if (opt == 1)
		tmp = add_env(myenv, name, content);
	else if (opt == 2)
		tmp = del_env(&myenv, name, NULL, myenv);
	else if (opt == 3)
		tmp = mod_env(&myenv, name, content);
	else if (opt == 4)
		free_env(myenv);
	return (tmp);
}