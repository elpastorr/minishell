/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 19:07:52 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/23 19:12:02 by elpastor         ###   ########.fr       */
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

t_cmd	*init_cmd(t_cmd *next, t_token *arg, t_token *redir)
{
	t_cmd	*data;

	data = malloc(sizeof(t_cmd));
	if (!data)
		return (NULL);
	data->arg = arg;
	data->redir = redir;
	data->next = next;
	data->fdin = 0;
	data->fdout = 1;
	data->pid = 0;
	return (data);
}

t_cmd	*cmd_init(t_cmd *res, t_token **tmp, t_token *token)
{
	t_token	*temp;
	t_cmd	*data;

	data = init_cmd(NULL, NULL, NULL);
	if (!data)
	{
		free_token(token);
		ctfree(res, "init token failed", 'c', 1);
		return (NULL);
	}
	temp = *tmp;
	*tmp = (*tmp)->next;
	temp->next = NULL;
	free_token(temp);
	return (data);
}

void	mod_env_extra(t_env *tmp, char *content, int opt)
{
	if (content && opt == 5)
		tmp->content = ft_strjoin_free(tmp->content, content, 2);
	else if (content && opt == 3)
	{
		free(tmp->content);
		tmp->content = ft_strdup(content);
	}
}
