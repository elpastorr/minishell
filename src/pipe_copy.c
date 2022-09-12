/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_copy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 17:28:58 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/12 22:37:51 by eleotard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	exec(t_cmd *cmd)
// {
// 	if (!cmd || !cmd->arg || !cmd->arg->str)
// 		exit_free(cmd, NULL, c, 1);
// 	else if (!ft_strcmp(cmd->arg->str, "echo"))
// 		exec_echo(cmd);
// 	else if (!ft_strcmp(cmd->arg->str, "cd"))
// 		exec_cd(cmd);
// 	else if (!ft_strcmp(cmd->arg->str, "pwd"))
// 		exec_pwd(cmd);
// 	else if (!ft_strcmp(cmd->arg->str, "export"))
// 		exec_export(cmd);
// 	else if (!ft_strcmp(cmd->arg->str, "unset"))
// 		exec_unset(cmd);
// 	else if (!ft_strcmp(cmd->arg->str, "env"))
// 		exec_env(cmd);
// 	else if (!ft_strcmp(cmd->arg->str, "exit"))
// 		exec_exit(cmd);
// 	else
// 		exe_cmd(cmd);
// }

/*void	parent(t_cmd *cmd)
{
	if (!get_nbpipe(cmd) && is_built(cmd))
	{
		print_cmd(cmd);
		// exec(cmd);
	}
}*/

int	get_cmd_size(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		i;

	i = 0;
	tmp = cmd;
	while (tmp)
	{
		tmp = tmp->next;
		i++;
	}
	printf("SIZE CMD = %d\n", i);
	return (i);
}

//void	close_fds_parent(int *i, int *j)


void	ft_pipe(t_cmd *cmd)
{
	int	pid;
	int i;
	int cmd_size;
	//int	fd[2][2];
	t_cmd	*tmp;
	int fd[2];
	int previous;
	
	fd[0] = 0;
	tmp = cmd;
	cmd_size = get_cmd_size(cmd);
	while (tmp)
	{
		previous = fd[0];
		printf("PREVIOUS = %d\n\n", previous);
		if (tmp->next)
		{
			if (pipe(fd) < 0)
			{
				perror("Pipe : ");
				return ;
			}
			if (tmp->fdout == 1)
				tmp->fdout = fd[1];
		}
		if (tmp->fdin == 0)
			tmp->fdin = previous;
		printf("cmd = %s\tfd[0] = %d\tfd[1] = %d\n", tmp->arg->str, fd[0], fd[1]);
		printf("cmd = %s\tfdin = %d\tfdout = %d\n\n", tmp->arg->str, tmp->fdin, tmp->fdout);
		pid = fork();
		if (pid < 0)
			break ;
		if (pid == 0)
		{
			if (tmp->fdin != 0)
			{
				dup2(tmp->fdin, 0);
				if (tmp->fdin == previous)
					close(tmp->fdin);
				else
					close(fd[0]);
			}	
			if (tmp->fdout != 1)
			{
				dup2(tmp->fdout, 1);
				if (tmp->fdout != fd[1])
					close(tmp->fdout);
				else
					close(fd[1]);
			}
			close(fd[0]);
			if (!tmp->next)
				close(fd[1]);
			determine_exe_type(tmp);
		}
		else
		{
			close(fd[1]);
			printf("\nPREVIOUS = %d\n----->close previous\n", previous);
			if (previous != 0)
				close(previous);
			tmp = tmp->next;
			if (!tmp)
				close(fd[0]);
		}
		
	}
	if (pid < 0)
		return ;
	i = -1;
	while (++i < cmd_size)
		waitpid(0,0,0);
}