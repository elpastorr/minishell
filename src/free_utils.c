/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 17:33:22 by eleotard          #+#    #+#             */
/*   Updated: 2022/09/16 18:20:25 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*free_tabtab(char **tab)
{
	int	k;

	k = 0;
	while (tab[k])
		k++;
	while (k >= 0)
	{
		free(tab[k]);
		k--;
	}
	free(tab);
	return (NULL);
}

void	free_tabs_exit_free(t_cmd *cmd, char **env, char **argv, char *err)
{
	if (env)
		free_tabtab(env);
	if (argv)
		free_tabtab(argv);
	exit_free(cmd, err, 'c', 1);
}