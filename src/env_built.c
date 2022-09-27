/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_built.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 16:24:14 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/24 16:09:36 by eleotard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ex_port_substr(t_token *arg, char **name, char **content)
{
	if (get_equal(arg->str) == -1)
	{
		*name = ft_substr(arg->str, 0, get_equal2(arg->str));
		*content = ft_substr(arg->str, (get_equal2(arg->str) + 2),
				ft_strlen(arg->str));
		handler(5, NULL, *name, *content);
	}
	else
	{
		*name = ft_substr(arg->str, 0, get_equal(arg->str));
		*content = ft_substr(arg->str, (get_equal(arg->str) + 1),
				ft_strlen(arg->str));
		handler(3, NULL, *name, *content);
	}
}

int	export_test_error(t_token *arg)
{
	if (get_equal(arg->str) == 0)
	{
		if (!ft_isalpha(arg->str[0]) && arg->str[0] != '_')
		{
			print_err("export: ", arg->str, ": not a valid identifier");
			return (1);
		}
	}
	return (0);
}

void	ex_port(t_cmd *cmd, int ret)
{
	t_token	*arg;
	char	*name;
	char	*content;

	if (!cmd->arg->next)
		return (ex_env(cmd));
	arg = cmd->arg->next;
	while (arg)
	{
		if (export_test_error(arg))
			ret = 1;
		else
		{
			ex_port_substr(arg, &name, &content);
			free(name);
			if (get_equal(arg->str) != -1)
				free(content);
		}
		arg = arg->next;
	}
	handler(ret, NULL, "?", NULL);
}

void	ex_unset(t_cmd *cmd)
{
	t_token	*arg;
	int		ret;

	ret = 0;
	if (!cmd->arg->next)
		return ;
	arg = cmd->arg->next;
	while (arg)
	{
		if (!ft_isalpha(arg->str[0]) && arg->str[0] != '_')
		{
			print_err("export: ", arg->str, ": not a valid identifier");
			ret = 1;
		}
		else
			handler(2, NULL, arg->str, NULL);
		arg = arg->next;
	}
	handler(ret, NULL, "?", NULL);
}

void	ex_env(t_cmd *cmd)
{
	t_env	*env;

	env = handler(3, NULL, NULL, NULL);
	while (env)
	{
		if (env->content)
		{
			ft_putstr_fd(env->name, cmd->fdout);
			write(cmd->fdout, "=", 1);
			ft_putstr_fd(env->content, cmd->fdout);
			write(cmd->fdout, "\n", 1);
		}
		env = env->next;
	}
	handler(0, NULL, "?", NULL);
}
