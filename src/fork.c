/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 17:00:57 by eleotard          #+#    #+#             */
/*   Updated: 2022/09/01 23:20:03 by eleotard         ###   ########.fr       */
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

/*int **create_fd_tab(int *fd)
{
    
}*/

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

char	*look_for_path(t_cmd *cmd) //si ya deja un slash faut checker si ca existe deja et si cest le cas il faut execve
{
	int		i;
	t_env	*whole_path;
    char	**tab_of_paths;
	char	*cmd_path;
	
	whole_path = handler(3, NULL, "PATH", NULL);
	printf("ligne entiere de chemins = [%s]\n", whole_path->content);
	tab_of_paths = ft_split(whole_path->content, ':');
	if (!tab_of_paths)
		return (ctfree(cmd, "ERREUR MALLOC EXEC PATH", 'c', -1), NULL); //on verra free cmd et reaffcher une nouvelle ligne, return NULL
	//ctfree(cmd, "ERREUR A", 'c', -1),
	i = 0;
	while (tab_of_paths[i])
	{
		cmd_path = join(tab_of_paths[i], join("/", cmd->arg->str));
		if(!cmd_path)
			return (free_tabtab(tab_of_paths), NULL);
		if (access(cmd_path, X_OK) == 0) // pour verifier si c'est bien un executable
		{
			printf("command path = [%s]\n", cmd_path);
			return (cmd_path);
		}
		i++;
	}    
    return (NULL);
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

void	*exec(t_cmd *cmd, const char *pathname)
{
	int		nb_of_arg;
	char	**argv;
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
		return (NULL);
	if (nb_of_arg == 0)
		return (free_tabtab(env), NULL);
	if (nb_of_arg == 1)
	{
		argv = malloc(sizeof(char *));
		argv[0] = strdup(cmd->arg->str);
		argv[1] = NULL;
		ctfree(cmd, NULL, 'c', 4);
		execve(pathname, argv, env);
	}
		
	else
	{
		argv = get_exec_args(cmd, nb_of_arg);
		print_tabtab(argv);
		if (!argv)
			return (free_tabtab(env), NULL);
		printf("%s - %s - %p\n", argv[0], argv[1], argv[2]);
		ctfree(cmd, NULL, 'c', 4);
		execve(pathname, argv, env);
	}
	return (NULL);
}


void	*child(t_cmd *cmd, int pid, int *fd) //besoin de malloc les fd pour ca
{
	char	*path;

	if (pid == 0) // on est dans l'enfant 
	{
		close(fd[0]);
		//dup2(fd[1], 1); //on remplace lentree standard par ca
        if (is_built(cmd))
            printf("C'est un built-in perso\n");
        else
        {
            path = look_for_path(cmd);
            if (!path)
                return (NULL);
			exec(cmd, path);
        }
    }
    else
    {
        close(fd[1]);
		wait(NULL);
    }
	return (NULL);
}

void	*parent(t_cmd *cmd)
{
    int fd[2];  //fd[nb_of_forks][2]
    int pid1;
    int nb_of_pipes;
    
    nb_of_pipes = find_nb_of_pipes(cmd);
    if (nb_of_pipes) //si ya des pipes
    {
        //fd = malloc(sizeof(int) * (nb_of_pipes * 2));
        if (pipe(fd) == -1)
            return (NULL);
        pid1 = fork();
        if (pid1 == -1)
            return (NULL);
        child(cmd, pid1, fd);
    }
    else //ya quand meme une commande donc il faut fork
    {
        printf("Il n'y a pas de pipes\n");
        if (pipe(fd) == -1)
			return (NULL);
		pid1 = fork();
		if (pid1 == -1)
			return (NULL);
		child(cmd, pid1, fd);
    }
    return (NULL);
}