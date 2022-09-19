/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 17:00:57 by eleotard          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2022/09/19 15:09:49 by elpastor         ###   ########.fr       */
=======
/*   Updated: 2022/09/16 16:36:58 by eleotard         ###   ########.fr       */
>>>>>>> 33d807d178f1a830004811a8cf3ab6aa1ce6accf
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_multi_pipe(t_cmd *cmd);

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

char	*find_path(t_cmd *cmd, char **tab_of_paths)
{
	int		i;
	char	*tmp;
	char	*cmd_path;

	i = 0;
	tmp = join("/", cmd->arg->str);
	if (!tmp)
		return (NULL);
	while (tab_of_paths[i])
	{
		cmd_path = join(tab_of_paths[i], tmp);
		if (!cmd_path)
			return (free(tmp), NULL);
		if (access(cmd_path, X_OK) == 0)
			return (free(tmp), cmd_path);
		free(cmd_path);
		cmd_path = NULL;
		i++;
	}
	return (free(tmp), NULL);
}

char	*look_for_path(t_cmd *cmd)
{
	t_env	*whole_path;
	char	**tab_of_paths;
	char	*cmd_path;

	whole_path = handler(3, NULL, "PATH", NULL);
	//printf("ligne entiere de chemins = [%s]\n", whole_path->content);
	tab_of_paths = ft_split(whole_path->content, ':');
	if (!tab_of_paths)
		return (ctfree(cmd, "ERREUR MALLOC EXEC PATH", 'c', 1), NULL);
	cmd_path = find_path(cmd, tab_of_paths);
	if (!cmd_path)
		return (free_tabtab(tab_of_paths));
	free_tabtab(tab_of_paths);
	return (cmd_path);
}

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

int		find_slash(t_cmd *cmd)
{
	t_token	*tmp;
	int		i;

	tmp = cmd->arg;
	i = 0;
	if (!tmp || !tmp->str)
		return (0);
	while (tmp->str[i])
	{
		if (tmp->str[i] == '/')
			return (1);
		i++;
	}
	return (0);
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

void	exec(t_cmd *cmd, const char *pathname)
{
	char	**argv;
	char	**env;
	int		nb_of_arg;

	env = get_exec_env();
	if (!env)
		exit_free(cmd, "\nERROR MALLOC ENV\n", 'c', 1);
	nb_of_arg = find_nb_of_args(cmd);
	if (nb_of_arg >= 1)
	{
		argv = get_exec_args(cmd, nb_of_arg);
		if (!argv)
			free_tabs_exit_free(cmd, env, argv, "WRONG COMMAND/NOT EXE\n");
		if (!strcmp(pathname, argv[0]))
			pathname = argv[0];
		ctfree(cmd, NULL, 'c', 0);
		if (execve(pathname, argv, env) == -1)
			exit(1);
	}
	else
		free_tabs_exit_free(cmd, env, NULL, "WRONG COMMAND/NOT EXE\n");
}

void	determine_exe_type(t_cmd *cmd) //besoin de malloc les fd pour ca
{
	char	*path;

	if (is_built(cmd))
		exec_built(cmd);
	else if (!is_built(cmd) && !find_slash(cmd))
	{
		path = look_for_path(cmd);
		if (!path)
			exit_free(cmd, "WRONG COMMAND/NOT EXE\n", 'c', 127);
		exec(cmd, path);
	}
	else if (!is_built(cmd) && find_slash(cmd))
	{
		if (access(cmd->arg->str, X_OK) == -1)
			exit_free(cmd, "WRONG COMMAND/NOT EXE\n", 'c', 127);
		exec(cmd, cmd->arg->str);
	}
	else
		exit_free(cmd, "WRONG COMMAND/NOT EXE\n", 'c', 127);
}

void	single_cmd_handler(t_cmd *cmd)
{
	t_token	*cur;

	if (cmd->fdin != 0)
	{
		dup2(cmd->fdin, 0);
		close(cmd->fdin);
	}
	if (cmd->fdout != 1)
		dup2(cmd->fdout, 1);
	cur = cmd->redir;
	while (cur)
	{
		if (cur->fd != 0)
			close(cur->fd);
		cur = cur->next;
	}
	determine_exe_type(cmd);
}

void	*parent(t_cmd *cmd)
{
	if (!is_exe(cmd))
		return (ctfree(cmd, NULL, 'c', 127), NULL);
	if (is_built(cmd) && get_cmd_size(cmd) == 1)
		determine_exe_type(cmd);
	else
	{
		if (get_cmd_size(cmd) > 1)
			ft_multi_pipe(cmd);
		else
		{
			cmd->pid = fork();
			if (cmd->pid < 0)
				return (NULL);
			if (cmd->pid == 0)
				single_cmd_handler(cmd);
			else
			{
				if (cmd->fdin != 0)
					close (cmd->fdin);
				if (cmd->fdout != 1)
					close (cmd->fdout);
				wait(NULL);
			}
		}
	}
	return (ctfree(cmd, NULL, 'c', 0), NULL);
}
