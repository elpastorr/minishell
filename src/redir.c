/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 16:10:01 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/12 19:01:12 by eleotard         ###   ########.fr       */
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
	t_token	*tmp;
	t_cmd	*cmd_tmp;

	cmd_tmp = cmd;
	while (cmd)
	{
		tmp = cmd->redir;
		while (tmp)
		{
			if (tmp->type == rout)
				cmd->fdout = open(tmp->next->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else if (tmp->type == rdout)
				cmd->fdout = open(tmp->next->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
			else if (tmp->type == rin) // peut etre add if (tmp->next->str)
				cmd->fdin = open(tmp->next->str, O_RDONLY);
			printf("cmd = %s\tcmd_.fdin = %d\n", cmd->arg->str, cmd->fdin);
			/*if (tmp->type == rout || tmp->type == rdout)
				tmp->fd = cmd->fdout;
			else if (tmp->type == rin)
				tmp->fd = cmd->fdin;*/
			file_err(tmp);
			tmp = tmp->next;
		}
		cmd = cmd->next;
	}
	
	return (cmd_tmp);
}