/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 15:53:29 by eleotard          #+#    #+#             */
/*   Updated: 2022/09/21 16:03:35 by eleotard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**create_env_tab(t_env *env, int nb_of_lines)
{
	int		i;
	char	*tmp;
	char	**env_tab;

	env_tab = malloc(sizeof(char *) * (nb_of_lines + 1));
	if (!env_tab)
		return (NULL);
	i = -1;
	tmp = NULL;
	while ((++i < nb_of_lines))
	{
		tmp = join(env->name, "=");
		if (!tmp)
			return (free_tabtab(env_tab), NULL);
		env_tab[i] = join(tmp, env->content);
		if (!env_tab[i])
			return (free_tabtab(env_tab), NULL);
		free(tmp);
		tmp = NULL;
		env = env->next;
	}
	env_tab[i] = NULL;
	return (env_tab);
}

char	**get_exec_env(void)
{
	t_env	*env;
	t_env	*temp;
	int		nb_of_lines;
	char	**env_tab;

	env = handler(3, NULL, NULL, NULL);
	temp = env;
	nb_of_lines = 0;
	while (temp)
	{
		temp = temp->next;
		nb_of_lines++;
	}
	env_tab = create_env_tab(env, nb_of_lines);
	if (!env_tab)
		return (NULL);
	return (env_tab);
}

char	**get_exec_args(t_cmd *cmd, int nb_of_arg)
{
	char	**argv;
	t_token	*tmp;
	int		i;

	argv = malloc(sizeof(char *) * (nb_of_arg + 1));
	if (!argv)
		return (NULL);
	tmp = cmd->arg;
	i = -1;
	while ((++i < nb_of_arg) && tmp)
	{
		argv[i] = ft_strdup(tmp->str);
		if (!argv[i])
			return (free_tabtab(argv), NULL);
		tmp = tmp->next;
	}
	argv[i] = NULL;
	return (argv);
}

int	find_nb_of_args(t_cmd *cmd)
{
	t_token	*tmp;
	int		nb_of_args;

	tmp = cmd->arg;
	nb_of_args = 0;
	while (tmp)
	{
		tmp = tmp->next;
		nb_of_args++;
	}
	return (nb_of_args);
}

int	is_exe(t_cmd *cmd)
{
	char	*path;

	if (is_built(cmd))
		return (1);
	else if (!is_built(cmd) && !find_slash(cmd))
	{
		path = look_for_path(cmd);
		if (path)
		{
			free(path);
			return (2);
		}
	}
	else if (!is_built(cmd) && find_slash(cmd))
	{
		if (access(cmd->arg->str, X_OK) == 0)
			return (3);
	}
	return (0);
}
