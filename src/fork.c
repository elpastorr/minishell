/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 17:00:57 by eleotard          #+#    #+#             */
/*   Updated: 2022/09/02 23:05:14 by eleotard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"





int find_nb_of_pipes(t_cmd *cmd)
{
    t_cmd   *tmp;
    int     i;

    i = 0;
    tmp = cmd;
    while (tmp->next != NULL)
    {
        i++;
        tmp = tmp->next;
    }
    return (i);
}

int is_built(t_cmd *cmd)
{
	if (cmd->arg->str && !ft_strcmp(cmd->arg->str, "pwd")) //strcmp
		return (1);
	else if (cmd->arg->str && !ft_strcmp(cmd->arg->str, "cd"))
		return (2);
	else if (cmd->arg->str && !ft_strcmp(cmd->arg->str, "echo"))
		return (3);
	else if (cmd->arg->str && !ft_strcmp(cmd->arg->str, "unset"))
		return (4);
	else if (cmd->arg->str && !ft_strcmp(cmd->arg->str, "export"))
		return (5);
	else if (cmd->arg->str && !ft_strcmp(cmd->arg->str, "env"))
		return (6);
	return (0);
}

int		is_exe(t_cmd *cmd)
{
	char	*path;
	
	if (is_built(cmd))
		return (1);
	else if (!is_built(cmd) && !find_slash(cmd))
	{
		path = look_for_path(cmd);
		if (look_for_path(cmd))
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
		if(!cmd_path)
			return (free(tmp), NULL);
		if (access(cmd_path, X_OK) == 0) //pour verifier si cest bien un executable
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
	printf("ligne entiere de chemins = [%s]\n", whole_path->content);
	tab_of_paths = ft_split(whole_path->content, ':');
	if (!tab_of_paths)
		return (ctfree(cmd, "ERREUR MALLOC EXEC PATH", 'c', -1), NULL);
	cmd_path = find_path(cmd, tab_of_paths);
	if (!cmd_path)
		return(free_tabtab(tab_of_paths));
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
	printf("premier arg str [%s]\n", tmp->str);
	printf("nb of arg [%d]\n", nb_of_arg);
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

int	find_slash(t_cmd *cmd)
{
	t_token	*tmp;
	int		i;

	tmp = cmd->arg;
	i = 0;
	if (!tmp)
		return (0);
	while (tmp->str[i])
	{
		if (tmp->str[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

void	exec_cmd_without_redir(t_cmd *cmd, const char *pathname, int nb_of_arg, char **env)
{
	char	**argv;
	
	if (nb_of_arg == 0)
	{
		free_tabtab(env);
		exit_free(cmd, "\nNO ARGS\n", 'c', 4);
	}
	else if (nb_of_arg == 1)
	{
		argv = malloc(sizeof(char *) * 2);
		if (!argv)
			free_tabs_exit_free(cmd, env, NULL, "\nMALLOC ERROR ARGV\n");
		argv[0] = strdup(cmd->arg->str);
		if (!argv[0])
			free_tabs_exit_free(cmd, env, argv, "\nMALLOC ERROR ARGV\n");
		argv[1] = NULL;
		if (!strcmp(pathname, argv[0]))
			pathname = argv[0];
		ctfree(cmd, NULL, 'c', 4); //on free les donnes malloc dans l'enfant (env et cmd)
		if (execve(pathname, argv, env) == -1) //execve free automatiquement char**env et arg
			exit(4);
	}
	else if (nb_of_arg > 1)
	{
		argv = get_exec_args(cmd, nb_of_arg);
		if (!argv)
			free_tabs_exit_free(cmd, env, argv, "WRONG COMMAND/NOT EXE\n");
		printf("%s - %s - %p\n", argv[0], argv[1], argv[2]);
		if (!strcmp(pathname, argv[0]))
			pathname = argv[0];
		ctfree(cmd, NULL, 'c', 4);
		if (execve(pathname, argv, env) == -1)
			exit(4);
	}
	else
		free_tabs_exit_free(cmd, env, NULL, "WRONG COMMAND/NOT EXE\n");
}

void	exec(t_cmd *cmd, const char *pathname)
{
	int		nb_of_arg;
	char	**env;
	t_token	*tmp;

	tmp = cmd->arg;
	nb_of_arg = 0;
	while (tmp)
	{
		tmp = tmp->next;
		nb_of_arg++;
	}
	env = get_exec_env();
	if (!env)
		exit_free(cmd, "\nERROR MALLOC ENV\n", 'c', 4);
	if (!cmd->redir)
		exec_cmd_without_redir(cmd, pathname, nb_of_arg, env);
	
}


void	child(t_cmd *cmd, int *fd) //besoin de malloc les fd pour ca
{
	char	*path;

	if (cmd->pid == 0) // on est dans l'enfant 
	{
		if (fd)
			close(fd[0]);
        if (is_built(cmd))
            printf("C'est un built-in perso\n");
        else if (!is_built(cmd) && !find_slash(cmd))
        {
            path = look_for_path(cmd);
            if (!path)
                exit_free(cmd, "WRONG COMMAND/NOT EXE\n", 'c', 4);
			exec(cmd, path);
        }
		else if (!is_built(cmd) && find_slash(cmd))
		{
			if (access(cmd->arg->str, X_OK) == -1) {
				printf("MAUVAIS EXECUTABLE\n");
				exit_free(cmd, "WRONG COMMAND/NOT EXE\n", 'c', 4);
			}
			exec(cmd, cmd->arg->str);
		}
		else
			exit_free(cmd, "WRONG COMMAND/NOT EXE\n", 'c', 4);
    }
}



void	*parent(t_cmd *cmd)
{
    int fd[2];  //fd[nb_of_forks][2]
    int nb_of_pipes;
    
	if (!is_exe(cmd))
		return (ctfree(cmd, NULL, 'c', 4), NULL);
    nb_of_pipes = find_nb_of_pipes(cmd);
    if (nb_of_pipes) //si ya des pipes
    {
        //fd = malloc(sizeof(int) * (nb_of_pipes * 2));
        if (pipe(fd) == -1)
            return (NULL);
        cmd->pid = fork();
        if (cmd->pid == -1)
            return (NULL);
		else if (cmd->pid == 0)
        	child(cmd, fd);
		else if (cmd->pid > 0) // ligne qui peut etre supp
			wait(NULL);
    }
    else
    {
        printf("Il n'y a pas de pipes\n");
		cmd->pid = fork();
		if (cmd->pid == -1)
			return (NULL);
		if (cmd->pid == 0)
		{
			printf("child pid = [%d]\n", cmd->pid);
			child(cmd, NULL);
		}
		else { printf("parent pid = [%d]\n", cmd->pid);
			wait(NULL);}
    }
    return (ctfree(cmd, NULL, 'c', 4), NULL);
}