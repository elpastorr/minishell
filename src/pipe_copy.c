/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_copy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 17:28:58 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/15 15:57:03 by eleotard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (tmp->fdin != 0) //donc si cest previous ou un file
	{
		dup2(tmp->fdin, 0);
		close(tmp->fdin);
	}	
	if (tmp->fdout != 1) //si cest fd[1] ou un file
	{
		dup2(tmp->fdout, 1);
		close(tmp->fdout);
	}
}

void	close_child_fds(t_cmd *tmp, int previous, int in, int out)
{
	t_token *cur;
	
	cur = tmp->redir;
	while (cur)
	{
		if (cur->fd != 0)
			close(cur->fd);
		cur = cur->next;
	}
	if (tmp->fdin != previous)
		close(previous);
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

void	parent_life(t_cmd *tmp, int previous, int in, int out)
{
	t_token *cur;
	
	cur = tmp->redir;
	while (cur)
	{
		if (cur->fd != 0)
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

void	is_built_pipe(t_cmd *tmp)
{
	dup_in_and_out(tmp);
	exec_built(tmp);
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
				ctfree(cmd, "minishel: pipe", 'c', 1);
				break;	
			}
			if (tmp->fdout == 1)
				tmp->fdout = fd[1];
		}
		if (tmp->fdin == 0)
			tmp->fdin = previous;
		printf("cmd = %s\tfd[0] = %d\tfd[1] = %d\n", tmp->arg->str, fd[0], fd[1]);
		printf("cmd = %s\tfdin = %d\tfdout = %d\n\n", tmp->arg->str, tmp->fdin, tmp->fdout);
		if (is_built(tmp))
			is_built_pipe(tmp);
		else
		{
			tmp->pid = fork();
			if (tmp->pid < 0)
				break ;
			if (tmp->pid == 0)
				child_life(tmp, previous, fd[0], fd[1]);
		}
		parent_life(tmp, previous, fd[0], fd[1]);
		tmp = tmp->next;
	}
	
	//if (tmp->pid < 0)
	//	return ;
	i = -1;
	while (++i < cmd_size)
		waitpid(0,0,0);
}