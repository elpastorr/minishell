/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 21:14:51 by eleotard          #+#    #+#             */
/*   Updated: 2022/09/23 17:47:08 by eleotard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipe_exec_child(t_cmd *cmd, t_cmd *tmp, int previous, int fd[2])
{
	reset_default_signals();
	if (is_built(tmp))
		is_built_pipe(cmd, tmp, previous, fd);
	else
		child_life(tmp, previous, fd[0], fd[1]);
}

t_cmd	*file_error_pipe(t_cmd *tmp, int previous, int fd[2])
{
	if (previous != 0)
		close(previous);
	close(fd[1]);
	if (!tmp->next)
		close(fd[0]);
	tmp = tmp->next;
	return (tmp);
}

int	check_wrong_fd_waitpid(t_cmd *cmd)
{
	t_cmd	*cmd_tmp;

	cmd_tmp = cmd;
	while (cmd_tmp->next)
		cmd_tmp = cmd_tmp->next;
	if (cmd_tmp->fdin == -1 || cmd_tmp->fdout == -1)
		return (1);
	return (0);
}
