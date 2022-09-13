/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 16:10:01 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/13 17:35:37 by eleotard         ###   ########.fr       */
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

t_cmd	*redir(t_cmd *cmd)
{
	t_token	*cur;
	t_cmd	*cmd_tmp;

	cmd_tmp = cmd;
	while (cmd)
	{
		cur = cmd->redir;
		while (cur)
		{
			if (cur->type == rout) //>
				cmd->fdout = open(cur->next->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else if (cur->type == rdout) //>>
				cmd->fdout = open(cur->next->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
			else if (cur->type == rin) //<
				cmd->fdin = open(cur->next->str, O_RDONLY);
			printf("cmd = %s\tcmd_.fdin = %d\n", cmd->arg->str, cmd->fdin);
			if (cur->type == rout || cur->type == rdout)
				cur->fd = cmd->fdout;
			else if (cur->type == rin)
				cur->fd = cmd->fdin;
			file_err(cur);
			cur = cur->next;
		}
		cmd = cmd->next;
	}
	return (cmd_tmp);
}