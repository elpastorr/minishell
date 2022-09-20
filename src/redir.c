/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 16:10:01 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/20 16:53:38 by elpastor         ###   ########.fr       */
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

void	redir_plus(t_token *token, t_cmd *cmd_tmp, t_cmd *cmd, int *hd)
{
	int	oui;

	if (token->type == rout)
		cmd_tmp->fdout = open(token->next->str, O_WRONLY |
			O_CREAT | O_TRUNC, 0644);
	else if (token->type == rdout)
		cmd_tmp->fdout = open(token->next->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (token->type == rin)
		cmd_tmp->fdin = open(token->next->str, O_RDONLY);
	else if (token->type == rdin && *hd == 0)
	{
		oui = dup(0); //on copie l'entree standard
		signal(SIGINT, here_handler_sigint); //on la ferme pour fermer l'entree de readline et donc fermer le heredoc
		cmd_tmp->fdin = heredoc(cmd_tmp, cmd);
		*hd = 1;
		dup2(oui, 0); // comme elle a ete fermee on la remplace par sa copie et la retrouve en tant qu'entree standard normale
		close(oui); //on ferme la copie
		catch_signals();
	}
	if (token->type == rout || token->type == rdout)
		token->fd = cmd_tmp->fdout;
	else if (token->type == rin || token->type == rdin)
		token->fd = cmd_tmp->fdin;
}

t_cmd	*redir(t_cmd *cmd, int hd)
{
	t_cmd	*cmd_tmp;
	t_token	*token_tmp;
	int		oui;

	cmd_tmp = cmd;
	while (cmd_tmp)
	{
		token_tmp = cmd_tmp->redir;
		hd = 0;
		while (token_tmp)
		{
			redir_plus(token_tmp, cmd_tmp, cmd, &hd);
			if (token_tmp->fd == -1)
				return (file_err(token_tmp, cmd_tmp), NULL);
			token_tmp = token_tmp->next;
		}
		cmd_tmp = cmd_tmp->next;
	}
	return (cmd);
}
