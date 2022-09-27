/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 14:31:27 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/24 18:33:33 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_heredoc(t_cmd *cmd)
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
		if (s[i] == '$' && (ft_isalnum(s[i + 1])
				|| s[i + 1] == '_' || s[i + 1] == '?' || s[i + 1] == '$'))
			s = expend_words(s, i);
		if (s[i] != '$')
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

int	fd_heredoc(char *s, t_cmd *cmd)
{
	int		fd;
	char	*file_name;

	fd = -1;
	file_name = ft_strdup("/tmp/.heredoc_file0.txt");
	if (!file_name)
		exit_free(cmd, "Malloc error", 'c', 1);
	while (fd == -1)
	{
		fd = open(file_name, O_CREAT | O_RDWR, 0777);
		if (fd_is_already_used(fd, cmd))
			fd = -1;
		if (fd != -1)
			write(fd, s, ft_strlen(s));
		else
			file_name[18]++;
	}
	close (fd);
	fd = open(file_name, O_RDONLY);
	unlink(file_name);
	if (file_name)
		free(file_name);
	if (s)
		free(s);
	return (fd);
}
