/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 16:10:01 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/23 17:43:19 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc(t_cmd *temp, t_cmd *cmd, t_hd *hd)
{
	char	*tmp;
	t_token	*redir;

	if (!temp || !temp->redir)
		return (-1);
	tmp = NULL;
	redir = temp->redir;
	while (redir)
	{
		if (redir->type == fin)
			tmp = heredoc_extra(redir, tmp, 0);
		redir = redir->next;
	}
	if (hd->rdin == 1)
		return (cmd->fdin);
	if (!tmp)
		tmp = ft_strdup("");
	return (fd_heredoc(tmp, cmd));
}

void	close_all_fds(t_cmd *cmd, int opt)
{
	t_cmd	*cmd_tmp;
	t_token	*cur;

	cmd_tmp = cmd;
	while (cmd_tmp)
	{
		cur = cmd_tmp->redir;
		while (cur)
		{
			if (cur->fd != 0 && cur->fd != -1 && opt == 1)
				close(cur->fd);
			else if (cur->fd != cmd_tmp->fdin && cur->fd != cmd_tmp->fdout
				&& cur->fd != 0 && cur->fd != -1 && opt == 0)
				close(cur->fd);
			cur = cur->next;
		}
		cmd_tmp = cmd_tmp->next;
	}
}

void	file_err(t_token *tmp, t_cmd *cmd)
{
	if (access(tmp->next->str, F_OK) != 0)
		print_err(tmp->next->str, ": No such file or directory", NULL);
	else
		print_err(tmp->next->str, ": Permission denied", NULL);
	close_all_fds(cmd, 1);
	handler(1, NULL, "?", NULL);
}

void	redir_plus(t_token *token, t_cmd *cmd_tmp, t_cmd *cmd, t_hd *hd)
{
	int	oui;

	if (token->type == rout && hd->rdout == 0)
		cmd_tmp->fdout = open(token->next->str, O_WRONLY
				| O_CREAT | O_TRUNC, 0644);
	else if (token->type == rdout && hd->rdout == 0)
		cmd_tmp->fdout = open(token->next->str, O_WRONLY
				| O_CREAT | O_APPEND, 0644);
	else if (token->type == rin && hd->rdin == 0)
		cmd_tmp->fdin = open(token->next->str, O_RDONLY);
	get_old_fd_heredoc(cmd, cmd_tmp, token, hd);
	if (token->type == rdin && hd->here == 0)
	{
		oui = dup(0);
		signal(SIGINT, here_handler_sigint);
		cmd_tmp->fdin = heredoc(cmd_tmp, cmd, hd);
		hd->here = 1;
		dup2(oui, 0);
		close(oui);
		catch_signals();
	}
	if ((token->type == rout || token->type == rdout) && hd->rdout == 0)
		token->fd = cmd_tmp->fdout;
	else if ((token->type == rin || token->type == rdin) && hd->rdin == 0)
		token->fd = cmd_tmp->fdin;
}

t_cmd	*redir(t_cmd *cmd)
{
	t_cmd	*cmd_tmp;
	t_token	*token_tmp;
	t_hd	hd;

	cmd_tmp = cmd;
	while (cmd_tmp)
	{
		init_hd_struct(&hd);
		token_tmp = cmd_tmp->redir;
		while (token_tmp)
		{
			redir_plus(token_tmp, cmd_tmp, cmd, &hd);
			if (token_tmp->fd == -1)
			{
				if (get_cmd_size(cmd) == 1)
					return (file_err(token_tmp, cmd_tmp), NULL);
				else
					set_error_hd(token_tmp, cmd_tmp, &hd);
			}
			token_tmp = token_tmp->next;
		}
		cmd_tmp = cmd_tmp->next;
	}
	return (cmd);
}
