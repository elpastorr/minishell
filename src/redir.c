/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 16:10:01 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/19 18:47:04 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc(t_cmd *temp, t_cmd *cmd)
{
	char	*tmp;
	t_token	*redir;

	if (!temp || !cmd->redir)
		return (-1);
	tmp = NULL;
	redir = temp->redir;
	while (redir)
	{
		if (redir->type == fin)
			tmp = heredoc_extra(redir, tmp, 0);
		redir = redir->next;
	}
	if (!tmp)
		tmp = ft_strdup("");
	return (fd_heredoc(tmp, cmd));
}

void	print_err(char *file, char *s)
{
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(s, 2);
	ft_putstr_fd("\n", 2);
}

void	close_all_fds(t_cmd *cmd, int opt)
{
	t_cmd	*cmd_tmp;
	t_token *cur;

	cmd_tmp = cmd;
	while (cmd_tmp)
	{
		cur = cmd_tmp->redir;
		while (cur)
		{
			if (cur->fd != 0 && opt == 1)
				close(cur->fd);
			else if (cur->fd != cmd_tmp->fdin && cur->fd != cmd_tmp->fdout
					&& cur->fd != 0 && opt == 0)
				close(cur->fd);
			cur = cur->next;
		}
		cmd_tmp = cmd_tmp->next;
	}
}

void	file_err(t_token *tmp, t_cmd *cmd)
{
	if (access(tmp->next->str, F_OK) != 0)
		print_err(tmp->next->str, ": No such file or directory");
	else
		print_err(tmp->next->str, ": Permission denied");
	close_all_fds(cmd, 1);
	handler(1, NULL, "?", NULL);
}

t_cmd	*redir(t_cmd *cmd, int hd)
{
	t_cmd	*cmd_tmp;
	t_token	*token_tmp;

	cmd_tmp = cmd;
	while (cmd_tmp)
	{
		token_tmp = cmd_tmp->redir;
		hd = 0;
		while (token_tmp)
		{
			if (token_tmp->type == rout)
				cmd_tmp->fdout = open(token_tmp->next->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else if (token_tmp->type == rdout)
				cmd_tmp->fdout = open(token_tmp->next->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
			else if (token_tmp->type == rin)
				cmd_tmp->fdin = open(token_tmp->next->str, O_RDONLY);
			else if (token_tmp->type == rdin && !hd)
			{
				reset_default_signals();
				cmd_tmp->fdin = heredoc(cmd_tmp, cmd);
				hd = 1;
				catch_signals();
			}
			if (token_tmp->type == rout || token_tmp->type == rdout)
				token_tmp->fd = cmd_tmp->fdout;
			else if (token_tmp->type == rin || token_tmp->type == rdin)
				token_tmp->fd = cmd_tmp->fdin;
			if (token_tmp->fd == -1)
				return (file_err(token_tmp, cmd_tmp), NULL);
			// printf("token fd : %d, cmd fd : %d\n", token_tmp->fd, cmd_tmp->fdin);
			token_tmp = token_tmp->next;
		}
		cmd_tmp = cmd_tmp->next;
	}
	return (cmd);
}