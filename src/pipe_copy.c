/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_copy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 17:28:58 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/13 16:55:38 by eleotard         ###   ########.fr       */
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
void dup_in_and_out(t_cmd *tmp, int previous, int in, int out)
{
	if (tmp->fdin != 0)
	{
		dup2(tmp->fdin, 0);
		if (tmp->fdin == previous)
			close(tmp->fdin);
		else
			close(in);
	}	
	if (tmp->fdout != 1)
	{
		dup2(tmp->fdout, 1);
		if (tmp->fdout != out)
			close(tmp->fdout);
		else
			close(out);
	}
}

void	close_child_fds(t_cmd *tmp, int previous, int in, int out)
{
	if (tmp->fdin != previous)
		close(previous);
	if (tmp->fdout != out)
		close(out);
	close(in);
	if (!tmp->next)
		close(out);
}

void	child_life(t_cmd *tmp, int previous, int in, int out)
{
	dup_in_and_out(tmp, previous, in, out);
	close_child_fds(tmp, previous, in, out);
	determine_exe_type(tmp);
}

void	parent_life(t_cmd *tmp, int previous, int in, int out)
{
	int	file_in;
	int	file_out;

	file_in = 0;
	file_out = 0;
	(void)in;
	
	if (tmp->fdin != previous)
	{
		file_in = tmp->fdin;
		close(previous);
	}
	if (tmp->fdout != out)
	{
		file_out = tmp->fdout;
		close(out);
	}
	close(out);
	if (file_in != 0) {
		printf("file in = %d\n", file_in);
		if (close(file_in) == -1)
			printf("\n\n1\n\n");}
	if (file_out != 1 && file_out != 0) {
		printf("file out = %d\n", file_out);
		if (close(file_out) == -1)
			printf("\n\n2\n\n");}
	//printf("\nPREVIOUS = %d\n----->close previous\n", previous);
	if (tmp->next)
	{
		if (previous != 0)
			if (close(previous) == -1)
				printf("\n\n3\n\n");
	}
	
	//print_cmd(cmd);
}

void	ft_pipe(t_cmd *cmd)
{
	int i;
	int cmd_size;
	t_cmd	*tmp;
	int fd[2];
	int previous;
	
	
	fd[0] = 0;
	tmp = cmd;
	//faire un close de tous les fichiers ouverts avant, garder jste le  dernier cmd->redir->fd
	cmd_size = get_cmd_size(cmd);
	while (tmp)
	{
		previous = fd[0];
		printf("PREVIOUS = %d\n\n", previous);
		if (tmp->next)
		{
			if (pipe(fd) < 0)
				return ;
			if (tmp->fdout == 1)
				tmp->fdout = fd[1];
		}
		if (tmp->fdin == 0)
			tmp->fdin = previous;
		printf("cmd = %s\tfd[0] = %d\tfd[1] = %d\n", tmp->arg->str, fd[0], fd[1]);
		printf("cmd = %s\tfdin = %d\tfdout = %d\n\n", tmp->arg->str, tmp->fdin, tmp->fdout);
		tmp->pid = fork();
		if (tmp->pid < 0)
			break ;
		if (tmp->pid == 0)
			child_life(tmp, previous, fd[0], fd[1]);	
		else
			parent_life(tmp, previous, fd[0], fd[1]);
		tmp = tmp->next;
		if (!tmp)
			close(fd[0]);
		
	}
	//if (tmp->pid < 0)
	//	return ;
	i = -1;
	while (++i < cmd_size)
		waitpid(0,0,0);
}