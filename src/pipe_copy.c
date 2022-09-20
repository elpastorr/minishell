/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_copy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 17:28:58 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/20 17:46:55 by eleotard         ###   ########.fr       */
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
	return (i);
}

void	parent_life(t_cmd *tmp, int previous, int in, int out)
{
	t_token	*cur;

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

void	dup_in_and_out(t_cmd *tmp)
{
	if (tmp->fdin != 0) //donc si cest previous ou un file
	{
		dup2(tmp->fdin, 0);
		//close(tmp->fdin);
	}
	if (tmp->fdout != 1) //si cest fd[1] ou un file
	{
		dup2(tmp->fdout, 1); //fd[1]
		//close(tmp->fdout);
	}
}

void	close_child_fds(t_cmd *tmp, int previous, int in, int out)
{
	t_token	*cur;

	cur = tmp->redir;
	while (cur)
	{
		if (cur->fd != 0)
			close(cur->fd);
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
	close_child_fds(tmp, previous, fd[0], fd[1]);
	exec_built(tmp);
	exit_free(cmd, NULL, 'c', 0);
}

int	pipe_and_attribute_fds(t_cmd *cmd, t_cmd *tmp, int *previous, int fd[2])
{
	*previous = fd[0];
	//printf("PREVIOUS = %d\n\n", previous);
	if (tmp->next)
	{
		if (pipe(fd) < 0)
		{
			ctfree(cmd, "minishell: pipe error", 'c', errno);
			return (1);
		}
		if (tmp->fdout == 1)
			tmp->fdout = fd[1];
	}
	return (0);
}

void	multi_pipe_loop(t_cmd *cmd, t_cmd *tmp, int fd[2])
{
	int	previous;

	while (tmp)
	{
		if (pipe_and_attribute_fds(cmd, tmp, &previous, fd) == 1)
			break ;
		if (tmp->fdin == 0)
			tmp->fdin = previous;
		tmp->pid = fork();
		if (tmp->pid < 0)
		{
			ctfree(cmd, "fork error", 'c', 1);
			break ;
		}
		signal(SIGINT, SIG_IGN);
		if (tmp->pid == 0)
		{
			reset_default_signals();
			if (is_built(tmp))
				is_built_pipe(cmd, tmp, previous, fd);
			else
				child_life(tmp, previous, fd[0], fd[1]);
		}
		if (tmp->pid != 0)
			parent_life(tmp, previous, fd[0], fd[1]);
		tmp = tmp->next;
	}
}

void	check_children_status(t_cmd *tmp, int *res)
{
	int	status;
	
	while (tmp)
	{
		waitpid(tmp->pid, &status, 0);
		tmp = tmp->next;
	}
	catch_signals();
	if (WIFEXITED(status))
		*res = 0;
	else
	{
		if (WIFSIGNALED(status))
		{
			*res = 128 + status;
			write(1, "\n", 1);
			printf("status = %d\n", status); 
		}
		else
			*res = 128 + WEXITSTATUS(status);
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
	multi_pipe_loop(cmd, tmp, fd);
	tmp = cmd;
	check_children_status(tmp, &res);
	return (res);
}
