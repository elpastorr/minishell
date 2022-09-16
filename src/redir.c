/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 16:10:01 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/16 17:27:11 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_err(char *file, char *s)
{
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(s, 2);
}

int		file_err(t_token *tmp)
{
	while (tmp)
	{
		if ((tmp->type == rout || tmp->type == rdout || tmp->type == rin) && tmp->fd <= 0)
		{
			if (access(tmp->next->str, F_OK) != 0)
				print_err(tmp->next->str, ": No such file or directory\n");
			else
				print_err(tmp->next->str, ": Permission denied\n");
			handler(1, NULL, "?", NULL);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

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

t_cmd	*redir(t_cmd *cmd)
{
	t_cmd	*temp;
	t_token	*tmp;

	temp = cmd;
	while (temp)
	{
		tmp = temp->redir;
		while (tmp)
		{
			if (tmp->type == rout)
				temp->fdout = open(tmp->next->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else if (tmp->type == rdout)
				temp->fdout = open(tmp->next->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
			else if (tmp->type == rin)
				temp->fdin = open(tmp->next->str, O_RDONLY);
			else if (tmp->type == rdin)
				temp->fdin = heredoc(temp, cmd);
			if (tmp->type == rout || tmp->type == rdout)
				tmp->fd = temp->fdout;
			else if (tmp->type == rin || tmp->type == rdin)
				tmp->fd = temp->fdin;
			if (file_err(tmp))
				return (NULL);
			tmp = tmp->next;
		}
		temp = temp->next;
	}
	return (cmd);
}