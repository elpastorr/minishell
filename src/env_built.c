/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_built.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 16:24:14 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/08 16:25:01 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		get_equal(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	if (!ft_isalpha(s[i]) && s[i++] != '_')
		return (0);
	while (s[i] && (ft_isalnum(s[i]) || s[i] == '=' || s[i] == '+' || s[i] == '_'))
	{
		if (s[i] == '=' && s[i - 1] == '+')
			return (-1);
		else if (s[i] == '=')
			return (i);
		i++;
	}
	return (0);
}

void	ex_port(t_cmd *cmd)
{
	t_token	*arg;
	char	*name;
	char	*content;

	if (cmd->arg->next)
		arg = cmd->arg->next;
	else
		ex_env(cmd);
	if (get_equal(arg->str) == 0)
		return ;
	if (get_equal(arg->str) == -1)
	{
		name = ft_substr(arg->str, 0, get_equal(arg->str));
		content = ft_substr(arg->str, (get_equal(arg->str) + 2), ft_strlen(arg->str));
		handler(5, NULL, name, content);
	}
	else
	{
		name = ft_substr(arg->str, 0, get_equal(arg->str));
		content = ft_substr(arg->str, (get_equal(arg->str) + 1), ft_strlen(arg->str));
		handler(3, NULL, name, content);
	}
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
