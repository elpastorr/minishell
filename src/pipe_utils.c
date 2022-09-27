/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 15:11:27 by eleotard          #+#    #+#             */
/*   Updated: 2022/09/23 22:49:27 by eleotard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	is_built_pipe(t_cmd *cmd, t_cmd *tmp, int previous, int fd[2])
{
	close_child_fds(tmp, previous, fd[0], fd[1]);
	exec_built(tmp, cmd);
	exit_free(cmd, NULL, 'c', get_exit());
}

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

void	dup_in_and_out(t_cmd *tmp)
{
	if (tmp->fdin != 0 && tmp->fdin != -1)
		dup2(tmp->fdin, 0);
	if (tmp->fdout != 1 && tmp->fdout != -1)
		dup2(tmp->fdout, 1);
}

void	close_child_fds(t_cmd *tmp, int previous, int in, int out)
{
	t_token	*cur;

	cur = tmp->redir;
	while (cur)
	{
		if (cur->fd != 0 && cur->fd != -1)
			close(cur->fd);
		cur = cur->next;
	}
	if (tmp->fdin != previous)
	{
		if (previous != 0)
			close(previous);
	}
	close(in);
	if (!tmp->next || tmp->fdout != out)
		close(out);
}

void	check_children_status(t_cmd *cmd, t_cmd *tmp, int *res)
{
	int		status;

	while (tmp)
	{
		waitpid(tmp->pid, &status, 0);
		tmp = tmp->next;
	}
	catch_signals();
	if (check_wrong_fd_waitpid(cmd))
		*res = 1;
	else
	{
		if (WIFEXITED(status))
			*res = WEXITSTATUS(status);
		else if (WIFSIGNALED(status) && status != 13)
		{
			*res = 128 + WTERMSIG(status);
			write(1, "\n", 1);
		}	
	}
}
