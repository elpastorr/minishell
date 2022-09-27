/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 21:23:53 by eleotard          #+#    #+#             */
/*   Updated: 2022/09/23 17:43:28 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_hd_struct(t_hd *hd)
{
	hd->here = 0;
	hd->rdin = 0;
	hd->rdout = 0;
}

void	set_error_hd(t_token *token_tmp, t_cmd *cmd_tmp, t_hd *hd)
{
	file_err(token_tmp, cmd_tmp);
	if (cmd_tmp->fdin == -1)
		hd->rdin = 1;
	if (cmd_tmp->fdout == -1)
		hd->rdout = 1;
}

void	get_old_fd_heredoc(t_cmd *cmd, t_cmd *cmd_tmp, t_token *token, t_hd *hd)
{
	t_token	*current;

	current = cmd->redir;
	if (token->type == rdin && hd->here == 1)
	{
		while (current && current->type != rdin)
			current = current->next;
		if (current)
			cmd_tmp->fdin = current->fd;
	}
}
