/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 16:42:51 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/19 18:26:38 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_cmd(t_cmd *cmd)
{
	int		i;
	t_cmd	*tmp;

	i = 0;
	tmp = cmd;
	while (tmp)
	{
		printf("CMD [%d]:\n", i++);
		if (tmp->arg)
		{
			printf("ARG: ");
			print_token(tmp->arg);
		}
		if (tmp->redir)
		{
			printf("REDIR: ");
			print_token(tmp->redir);
		}
		printf("fdin = %d\n", tmp->fdin);
		printf("fdout = %d\n", tmp->fdout);
		tmp = tmp->next;
	}
}

t_token	*cmd_redir(t_token **tmp)
{
	t_token	*temp;
	t_token	*stop;

	temp = *tmp;
	if (*tmp)
	{
		if ((*tmp)->type == pip)
			return (NULL);
		while ((*tmp)->next && (*tmp)->next->type != word
			&& (*tmp)->next->type != pip)
			(*tmp) = (*tmp)->next;
	}
	stop = temp;
	while (stop != *tmp)
		stop = stop->next;
	if (*tmp)
		*tmp = (*tmp)->next;
	if (stop)
		stop->next = NULL;
	return (temp);
}

t_token	*cmd_arg(t_token **tmp)
{
	t_token	*temp;
	t_token	*stop;

	temp = *tmp;
	if (*tmp)
	{
		if ((*tmp)->type == pip)
			return (NULL);
		while ((*tmp)->next && (*tmp)->next->type == word)
			(*tmp) = (*tmp)->next;
	}
	stop = temp;
	while (stop != *tmp)
		stop = stop->next;
	if (*tmp)
		*tmp = (*tmp)->next;
	if (stop)
		stop->next = NULL;
	return (temp);
}

void	add_cmd(t_token **tmp, t_cmd *data)
{
	t_token	*temp;

	if ((*tmp)->type != word && !data->redir)
		data->redir = cmd_redir(tmp);
	else if ((*tmp)->type != word && data->redir)
	{
		temp = data->redir;
		while (temp->next)
			temp = temp->next;
		temp->next = cmd_redir(tmp);
	}
	else if ((*tmp)->type == word && !data->arg)
		data->arg = cmd_arg(tmp);
	else if ((*tmp)->type == word && data->arg)
	{
		temp = data->arg;
		while (temp->next)
			temp = temp->next;
		temp->next = cmd_arg(tmp);
	}
}

t_cmd	*pars_err(t_cmd *cmd)
{
	t_token	*r;

	if (cmd && cmd->redir)
	{
		r = cmd->redir;
		while (r)
		{
			if ((r->type == word || r->type == pip)
				|| (r->type > 3 && r->next && r->next->type >= 3))
			{
				printf("Minishell: parse error near '%s'\n", r->str);
				ctfree(cmd, NULL, 'c', 2);
				return (NULL);
			}
			r = r->next;
		}
	}
	return (cmd);
}
