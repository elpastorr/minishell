/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 15:02:15 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/20 21:12:34 by eleotard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token(t_token *token)
{
	t_token	*tmp;

	if (token)
	{
		while (token)
		{
			if (token->str)
				free(token->str);
			tmp = token;
			token = token->next;
			free(tmp);
		}
	}
}

void	free_cmd(t_cmd *cmd)
{
	t_cmd	*tmp;

	while (cmd)
	{
		if (cmd->arg)
			free_token(cmd->arg);
		if (cmd->redir)
			free_token(cmd->redir);
		tmp = cmd;
		cmd = cmd->next;
		free(tmp);
	}
	cmd = NULL;
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	if (!env)
		return ;
	while (env)
	{
		if (env->name)
			free(env->name);
		if (env->content)
			free(env->content);
		tmp = env;
		env = env->next;
		free(tmp);
	}
	env = NULL;
}

void	ctfree(void *ptr, char *err, char type, int n)
{
	if (type == 't')
		free_token(ptr);
	if (type == 'c')
		free_cmd(ptr);
	if (err)
		print_err(err, NULL, NULL);
	handler(n, NULL, "?", NULL);
}

void	exit_free(void *ptr, char *err, char type, int n)
{
	handler(4, NULL, NULL, NULL);
	if (type == 't')
		free_token(ptr);
	else if (type == 'c')
		free_cmd(ptr);
	if (err)
		print_err(err, NULL, NULL);
	rl_clear_history();
	exit(n);
}
