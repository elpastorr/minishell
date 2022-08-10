/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 19:20:09 by elpastor          #+#    #+#             */
/*   Updated: 2022/08/10 19:33:53 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_name(char *env)
{
	int	i;

	i = 0;
	if (tmp)
	{
		while (tmp[i] && tmp[i] != '=')
			i++;
	}
	if (!tmp || !tmp[i])
		return (NULL);
	return (ft_strndup(tmp, i));
}

char	*get_content(char *env)
{
	while (env != NULL && *env && *env != '=')
		env++;
	if (!env)
		return (NULL);
	env++;
	return (ft_strdup(env));
}