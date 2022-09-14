/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 14:31:27 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/14 19:02:19 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_heredoc(t_cmd *cmd)
{
	t_token	*redir;

	redir = cmd->redir;
	while (redir)
	{
		if (redir->type == rdin)
			return (1);
		redir = redir->next;
	}
	return (0);
}

char	*heredoc_strcat(char *tmp, char *s)
{
	int		i;
	int		j;
	char	*heredoc;

	heredoc = malloc(ft_strlen(tmp) + ft_strlen(s) + 2);
	if (!heredoc)
		return (NULL);
	i = 0;
	j = 0;
	if (tmp)
	{
		while (tmp && heredoc && tmp[i])
			heredoc[j++] = tmp[i++];
		free(tmp);
	}
	j = 0;
	if (s)
	{
		while (s && heredoc && s[j])
			heredoc[i++] = s[j++];
		free(s);
	}
	heredoc[i++] = '\n';
	heredoc[i] = '\0';
	return (heredoc);
}

char	*read_heredoc(char *s, char *tmp)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if (s[i] == '$' && !quot_status(s, i) && (ft_isalnum(s[i + 1])
			|| s[i + 1] == '_' || s[i + 1] == '?' || s[i + 1] == '$'))
			s = expend_words(s, i);
		i++;
	}
	return (heredoc_strcat(tmp, s));
}

char	*heredoc_extra(t_token *redir, char *tmp, int ret)
{
	char	*s;

	s = NULL;
	if (tmp)
	{
		free(tmp);
		tmp = NULL;
	}
	redir->str = del_unused_quot(redir->str);
	while (ret == 0)
	{
		s = readline("\e[1m\e[31m\002""> ""\001\e[0m\002");
		if (s && ft_strcmp(redir->str, s) != 0)
			tmp = read_heredoc(s, tmp);
		else
			ret = 1;
	}
	if (s)
		free(s);
	return (tmp);
}

char	*heredoc(t_cmd *cmd)
{
	char	*tmp;
	t_token	*redir;

	if (!cmd || !cmd->redir)
		return (NULL);
	tmp = NULL;
	redir = cmd->redir;
	while (redir)
	{
		if (redir->type == fin)
			tmp = heredoc_extra(redir, tmp, 0);
		redir = redir->next;
	}
	if (!tmp)
		tmp = ft_strdup("");
	return (tmp);
}

// int	fd_heredoc(char *s)
// {
// 	int	fd;

// 	fd = open("heredoc_file", );
// }