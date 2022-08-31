/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   family.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 16:42:01 by elpastor          #+#    #+#             */
/*   Updated: 2022/08/30 18:00:05 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_state = 0;

static void	wait_child(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		status;
	int		i;

	tmp = cmd;
	status = 0;
	i = 0;
	while (tmp)
	{
		if (!is_built(tmp))
		{
			if (tmp->pid && waitpid(tmp->pid, &status, 0) == -1)
				write(STDERR_FILENO, "ERROR\n", 6);
			if (WIFEXITED(status))//child exited normaly
				tmp->exit = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))//child exited with a signal
				tmp->exit = WTERMSIG(status)/* + 128 ??*/;
		}
		if (!tmp->next)
			handler(tmp->exit, NULL, "?", NULL);
		tmp = tmp->next;
	}
}

void	child_extra(t_cmd *cmd, t_cmd *tmp, int *pipefd, char *h_doc)
{
	signal(SIGQUIT, SIG_DFL);
	if (tmp->next)
		dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
	exec(tmp, h_doc);
	exit_free(cmd, NULL, 'c', 1);
}

void	child(t_cmd *cmd, t_cmd *tmp, int *pipefd, int i)
{
	int		fd_in;
	char	*h_doc;

	h_doc = NULL;
	fd_in = dup(STDIN_FILENO);
	while (tmp && pipefd)
	{
		if (is_heredoc(tmp))
			h_doc = heredoc(tmp);
		pipe(&pipefd[i * 2]);
		tmp->pid = fork();
		if (tmp->pid == 0)
		{
			if (get_nbpipe(cmd) != get_nbpipe(tmp))
				dup2(fd_in, STDIN_FILENO);
			child_extra(cmd, tmp, &pipefd[i * 2], h_doc);
		}
		if (h_doc)
			ft_memdel((void **)&h_doc);
		dup2(pipefd[i * 2], fd_in);
		close(pipefd[i * 2]);
		close(pipefd[(i++) * 2 + 1]);
		tmp = tmp->next;
	}
	close(fd_in);
}

void	parent(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		*pipefd;
	char	*h_doc;

	h_doc = NULL;
	g_state = 1;
	tmp = cmd;
	handler(0, NULL, "?", NULL);
	pipefd = malloc((get_nbpipe(cmd) * 2) * sizeof(int));
	if (!pipefd)
		return ;
	if (get_nbpipe(cmd) == 1 && is_built(cmd) && is_heredoc(cmd))
		h_doc = heredoc(cmd);
	if (get_nbpipe(cmd) == 1 && is_built(cmd))
		exec(cmd, h_doc);//A FAIRE
	else
	{
		child(cmd, tmp, pipefd, 0);
		wait_child(cmd);
	}
	g_state = 0;
	if (pipefd)
		free(pipefd);
	if (cmd)
		free_cmd(cmd);
}