/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 17:28:58 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/10 15:40:10 by eleotard         ###   ########.fr       */
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
	int	i;
	int	j;
	int	pid;
	int cmd_size;
	int	fd[2][2];
	t_cmd	*tmp;

	i = 1;
	j = 0;
	tmp = cmd;
	cmd_size = get_cmd_size(cmd);
	while (tmp)
	{
		if (i == 1)
			i = 0;
		else if (i == 0)
			i = 1;
		if (pipe(fd[i]) < 0)
			return ;//exit error
		if (tmp->next)
		{
			if (tmp->fdout == 1)
				tmp->fdout = fd[i][1];
			if (tmp->next->fdin == 0)
				tmp->next->fdin = fd[i][0];
		}
		printf("cmd = %s\tfd[i][0] = %d\tfd[i][1] = %d\ti = %d\n", tmp->arg->str, fd[i][0], fd[i][1], i);
		printf("cmd = %s\tfdin = %d\tfdout = %d\ti = %d\n\n", tmp->arg->str, tmp->fdin, tmp->fdout, i);
		pid = fork();
		if (pid < 0)
			break ;
		if (pid == 0)
		{
			if (tmp->fdin != 0)
			{
				dup2(tmp->fdin, 0);
				close(tmp->fdin);
			}	
			if (tmp->fdout != 1)
			{
				dup2(tmp->fdout, 1);
				close(tmp->fdout);
			}
			if (i == 0)
			{
				close(fd[0][0]);
				if (!tmp->next)
					close(fd[0][1]);
			}
			else if (i == 1)
			{
				close(fd[1][0]);
				if (!tmp->next)
					close(fd[1][1]);
			}
			determine_exe_type(tmp);
		}
		else 
		{
			if (i == 0)
			{
				if (j == 0)
					close(fd[0][1]);
				else if (j > 0 && j != cmd_size - 1)
				{
					close(fd[1][0]);
					close(fd[0][1]);
				}
				else if (j == cmd_size - 1)
				{
					close(fd[1][0]);
					close(fd[0][1]);
					close(fd[0][0]);
				}
			}
			else if (i == 1)
			{
				if (j > 0 && j != cmd_size - 1)
				{
					close(fd[0][0]);
					close(fd[1][1]);
				}
				else if (j == cmd_size - 1)
				{
					close(fd[0][0]);
					close(fd[1][1]);
					close(fd[1][0]); //et la //a voir si faut pas tout close ici
				}
			}
		}
		j++;
		tmp = tmp->next;
	}
	if (pid < 0)
		return ;
	i = -1;
	while (++i < cmd_size)
		waitpid(0,0,0);
}