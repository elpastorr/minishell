/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 16:37:42 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/06 18:41:55 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_built(t_cmd *cmd)
{
	if (!cmd || !cmd->arg || !cmd->arg->str)
		return (0);
	if (!strcmp(cmd->arg->str, "echo"))
		return (1);
	if (!strcmp(cmd->arg->str, "cd"))
		return (2);
	if (!strcmp(cmd->arg->str, "pwd"))
		return (3);
	if (!strcmp(cmd->arg->str, "export"))
		return (4);
	if (!strcmp(cmd->arg->str, "unset"))
		return (5);
	if (!strcmp(cmd->arg->str, "env"))
		return (6);
	if (!strcmp(cmd->arg->str, "exit"))
		return (7);
	return (0);
}

void	exec_built(t_cmd *cmd)
{
	if (is_built(cmd) == 1)
		ex_echo(cmd);
	// else if (is_built(cmd) == 2)
		// ex_cd(cmd);
	else if (is_built(cmd) == 3)
		ex_pwd(cmd);
	// else if (is_built(cmd) == 4)
		// ex_port(cmd);
	// else if (is_built(cmd) == 5)
		// ex_unset(cmd);
	else if (is_built(cmd) == 6)
		ex_env(cmd);
	// else if (is_built(cmd) == 7)
	// 	ex_it(cmd);
}

int	only_n(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	ex_echo(t_cmd *cmd)
{
	t_token	*arg;
	int		n;

	arg = cmd->arg->next;
	n = 0;
	while (arg)
	{
		if (arg && arg->str && !ft_strncmp(arg->str, "-n", 2) && only_n(&arg->str[1]))
			n = 1;
		else
		{
			ft_putstr_fd(arg->str, cmd->fdout);
			if (arg->next)
				write(cmd->fdout, " ", 1);
		}
		arg = arg->next;
	}
	if (!n)
		write(cmd->fdout, "\n", 1);
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

int		get_equal(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		if (s[i] == '=')
			return (i);
		i++;
	}
	return (0);
}

void	ex_port(t_cmd *cmd)//PAS FINIT
{
	t_token	*arg;
	char	*name;
	char	*content;

	printf("HOLA");
	if (cmd->arg->next)
		arg = cmd->arg->next;
	else
		ex_env(cmd);
	if (get_equal(arg->str) < 1)
		return ;
	name = ft_substr(arg->str, 0, get_equal(arg->str));
	content = ft_substr(arg->str, get_equal(arg->str), ft_strlen(arg->str));
	printf("%s=%s\n", name, content);
	// handler(3, NULL, name, content);
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
