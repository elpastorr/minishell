/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 17:28:58 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/24 18:49:19 by eleotard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parent_life(t_cmd *tmp, int previous, int in, int out)
{
	t_token	*cur;

	cur = tmp->redir;
	while (cur)
	{
		if (cur->fd != 0 && cur->fd != -1)
			close(cur->fd);
		cur = cur->next;
	}
	close(out);
	if (tmp->next)
	{
		if (previous != 0)
			close(previous);
	}
	if (!tmp->next || tmp->fdin == -1 || tmp->fdout == -1)
		close(in);
}

void	child_life(t_cmd *tmp, int previous, int in, int out)
{
	dup_in_and_out(tmp);
	close_child_fds(tmp, previous, in, out);
	determine_exe_type(tmp, NULL);
}

int	pipe_and_attribute_fds(t_cmd *cmd, t_cmd *tmp, int *previous, int fd[2])
{
	*previous = fd[0];
	if (tmp->next)
	{
		if (pipe(fd) < 0)
		{
			if (*previous != 0)
				close(*previous);
			ctfree(cmd, "minishell: pipe error", 'c', 1);
			return (1);
		}
		if (tmp->fdout == 1)
			tmp->fdout = fd[1];
	}
	return (0);
}

void	multi_pipe_loop(t_cmd *cmd, t_cmd *tmp, int fd[2], int previous)
{
	while (tmp)
	{
		if (pipe_and_attribute_fds(cmd, tmp, &previous, fd) == 1)
			break ;
		if (tmp->fdin == -1 || tmp->fdout == -1)
			tmp = file_error_pipe(tmp, previous, fd);
		else
		{
			if (tmp->fdin == 0)
				tmp->fdin = previous;
			tmp->pid = fork();
			if (tmp->pid < 0)
			{
				ctfree(cmd, "fork error", 'c', 1);
				break ;
			}
			signal(SIGINT, SIG_IGN);
			signal(SIGQUIT, SIG_IGN);
			if (tmp->pid == 0)
				pipe_exec_child(cmd, tmp, previous, fd);
			if (tmp->pid != 0)
				parent_life(tmp, previous, fd[0], fd[1]);
			tmp = tmp->next;
		}
	}
}

int	ft_multi_pipe(t_cmd *cmd)
{
	int		res;
	t_cmd	*tmp;
	int		fd[2];

	res = 0;
	fd[0] = 0;
	tmp = cmd;
	multi_pipe_loop(cmd, tmp, fd, 0);
	tmp = cmd;
	check_children_status(cmd, tmp, &res);
	return (res);
}
