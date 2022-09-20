/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_built.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 16:24:14 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/20 17:42:46 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ex_port(t_cmd *cmd)
{
	t_token	*arg;
	char	*name;
	char	*content;

	if (cmd->arg->next)
		arg = cmd->arg->next;
	else
		return (ex_env(cmd));
	if (get_equal(arg->str) == 0)
		return ;
	ex_port_substr(arg, &name, &content);
	free(name);
	free(content);
}

void	ex_unset(t_cmd *cmd)
{
	t_token	*arg;

	if (!cmd->arg->next)
		return ;
	arg = cmd->arg->next;
	while (arg)
	{
		handler(2, NULL, arg->str, NULL);
		arg = arg->next;
	}
}

void	ex_env(t_cmd *cmd)
{
	t_env	*env;

	env = handler(3, NULL, NULL, NULL);
	while (env)
	{
		ft_putstr_fd(env->name, cmd->fdout);
		write(cmd->fdout, "=", 1);
		ft_putstr_fd(env->content, cmd->fdout);
		write(cmd->fdout, "\n", 1);
		env = env->next;
	}
}

void	ex_pwd(t_cmd *cmd)
{
	char	*s;
	char	buf[4096];

	s = getcwd(buf, 4096);
	if (s)
		ft_putstr_fd(s, cmd->fdout);
	write(cmd->fdout, "\n", 1);
}
