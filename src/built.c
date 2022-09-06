/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 16:37:42 by elpastor          #+#    #+#             */
/*   Updated: 2022/08/31 17:44:14 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_built(t_cmd *cmd)
{
	if (!cmd || !cmd->arg || !cmd->arg->str)
		return (0);
	if (!strcmp(cmd->arg->str, "echo"))
		return (1);
	if (!strcmp(cmd->arg->str, "cd"))
		return (2);
	if (!strcmp(cmd->arg->str, "pwd"))
		return (3);
	if (!strcmp(cmd->arg->str, "export"))
		return (4);
	if (!strcmp(cmd->arg->str, "unset"))
		return (5);
	if (!strcmp(cmd->arg->str, "env"))
		return (6);
	if (!strcmp(cmd->arg->str, "exit"))
		return (7);
	return (0);
}
