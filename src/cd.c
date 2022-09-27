/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 21:44:27 by eleotard          #+#    #+#             */
/*   Updated: 2022/09/24 19:15:14 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_nb_arg_cd(t_cmd *cmd)
{
	if (cmd->arg->next && cmd->arg->next->next)
	{
		handler(1, NULL, "?", NULL);
		print_err("cd: ", "too many arguments", NULL);
		return (1);
	}
	return (0);
}

void	actualize_pwd_and_old_pwd(void)
{
	static int	i = 0;
	char		buf[4096];

	if (i == 0)
	{
		handler(3, NULL, "PWD", getcwd(buf, 4096));
		i++;
		return ;
	}
	else
	{
		handler(3, NULL, "OLDPWD", handler(3, NULL, "PWD", NULL)->content);
		handler(3, NULL, "PWD", getcwd(buf, 4096));
	}	
}

void	ex_cd(t_cmd *cmd, t_env *env)
{
	char	*s;
	int		f;

	f = 0;
	if (cmd->arg->next)
		s = cmd->arg->next->str;
	else
		s = NULL;
	if (check_nb_arg_cd(cmd))
		return ;
	if (!env && (!s || s[0] == '~'))
		return (print_err("cd : HOME not set", NULL, NULL));
	ex_cd_plus(env, &s, &f);
	if (s && chdir(s) == -1)
	{
		handler(1, NULL, "?", NULL);
		print_err("cd: ", s, ": No such file or directory");
	}
	else
		actualize_pwd_and_old_pwd();
	if (f)
		free(s);
}
