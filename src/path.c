/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 15:55:43 by eleotard          #+#    #+#             */
/*   Updated: 2022/09/23 15:01:09 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_slash(t_cmd *cmd)
{
	t_token	*tmp;
	int		i;

	tmp = cmd->arg;
	i = 0;
	if (!tmp || !tmp->str)
		return (0);
	while (tmp->str[i])
	{
		if (tmp->str[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

char	*find_path(t_cmd *cmd, char **tab_of_paths)
{
	int		i;
	char	*tmp;
	char	*cmd_path;

	i = 0;
	if (!cmd->arg)
		return (NULL);
	tmp = join("/", cmd->arg->str);
	if (!tmp)
		return (NULL);
	while (tab_of_paths[i])
	{
		cmd_path = join(tab_of_paths[i], tmp);
		if (!cmd_path)
			return (free(tmp), NULL);
		if (access(cmd_path, X_OK) == 0)
			return (free(tmp), cmd_path);
		free(cmd_path);
		cmd_path = NULL;
		i++;
	}
	return (free(tmp), NULL);
}

char	*look_for_path(t_cmd *cmd)
{
	t_env	*whole_path;
	char	**tab_of_paths;
	char	*cmd_path;

	whole_path = handler(3, NULL, "PATH", NULL);
	if (!whole_path)
		return (NULL);
	tab_of_paths = ft_split(whole_path->content, ':');
	if (!tab_of_paths)
		return (ctfree(cmd, "ERREUR MALLOC EXEC PATH", 'c', 1), NULL);
	cmd_path = find_path(cmd, tab_of_paths);
	if (!cmd_path)
		return (free_tabtab(tab_of_paths));
	free_tabtab(tab_of_paths);
	return (cmd_path);
}
