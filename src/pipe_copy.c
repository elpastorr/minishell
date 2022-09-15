/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_copy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 17:28:58 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/15 20:59:02 by eleotard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>

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

void dup_in_and_out(t_cmd *tmp)
{
	printf("est dans le dup\n");
	if (tmp->fdin != 0) //donc si cest previous ou un file
	{
		dup2(tmp->fdin, 0);
		//sleep(10);
		//close(tmp->fdin);
	}
	printf("est dans le dup22222\n");
	if (tmp->fdout != 1) //si cest fd[1] ou un file
	{
		dup2(tmp->fdout, 1); //fd[1]
		//sleep(10);
		//close(tmp->fdout);
	}
}

void	close_child_fds(t_cmd *tmp, int previous, int in, int out)
{
	t_token *cur;
	
	(void)previous;
	cur = tmp->redir;
	if (!cur)
		printf("cur est NULL");
	while (cur)
	{
		printf("est rentre dans la boucle\n");
		if (cur->fd != 0 && cur->fd != tmp->fdin && cur->fd != tmp->fdout)
		{
			printf("closed fd : %d\n", cur->fd);
			close(cur->fd);
		}
		cur = cur->next;
	}
	if (tmp->fdin != previous)
	{
		if (previous != 0)
			close(previous);
	}
	close(in); //fd[0]
	if (!tmp->next || tmp->fdout != out)
		close(out);
}

void	child_life(t_cmd *tmp, int previous, int in, int out)
{
	dup_in_and_out(tmp);
	close_child_fds(tmp, previous, in, out);
	determine_exe_type(tmp);
}

void	is_built_pipe(t_cmd *cmd, t_cmd *tmp, int previous, int fd[2])
{
	printf("\nEST PAAAaaAA\n");
	//dup_in_and_out(tmp);
	close_child_fds(tmp, previous, fd[0], fd[1]);
	printf("\n2\n");
	exec_built(tmp);
	exit_free(cmd, NULL, 'c', 0);
}

void	parent_life(t_cmd *tmp, int previous, int in, int out)
{
	t_token *cur;
	
	//dup2(0, STDIN_FILENO);
	//dup2(1, STDOUT_FILENO);
	cur = tmp->redir;
	while (cur)
	{
		if (cur->fd != 0 && cur->fd != tmp->fdin && cur->fd != tmp->fdout)
			close(cur->fd);
		cur = cur->next;
	}
	close(out);
	if (tmp->next)
	{
		if (previous != 0)
			close(previous);
	}
	if (!tmp->next)
		close(in);
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
			{
				ctfree(cmd, "minishell: pipe error", 'c', errno);
				break;	
			}
			if (tmp->fdout == 1)
				tmp->fdout = fd[1];
		}
		if (tmp->fdin == 0)
			tmp->fdin = previous;
		printf("cmd = %s\tfd[0] = %d\tfd[1] = %d\n", tmp->arg->str, fd[0], fd[1]);
		printf("cmd = %s\tfdin = %d\tfdout = %d\n\n", tmp->arg->str, tmp->fdin, tmp->fdout);
		//if (is_built(tmp))
		//	is_built_pipe(tmp);
		//else
		//{
			tmp->pid = fork();
			if (tmp->pid < 0)
			{
				ctfree(cmd, "minishell: fork error", 'c', 1);
				break;	
			}
			if (tmp->pid == 0)
			{
				if (is_built(tmp))
					is_built_pipe(cmd, tmp, previous, fd);
				else	
					child_life(tmp, previous, fd[0], fd[1]);
			}
		//}
		if (tmp->pid != 0)
			parent_life(tmp, previous, fd[0], fd[1]);
		tmp = tmp->next;
	}
	
	//if (tmp->pid < 0)
	//	return ;
	i = -1;
	while (++i < cmd_size)
		waitpid(0,0,0);
}