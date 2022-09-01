/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 17:23:56 by eleotard          #+#    #+#             */
/*   Updated: 2022/09/01 19:33:39 by eleotard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_m(char *base, char *read)
{
	char	*line;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!base || !read)
		return (NULL);
	line = malloc(sizeof(char) * (ft_strlen(base) + ft_strlen(read) + 2));
	if (!line)
		return (NULL);
	while (base[i])
	{
		line[i] = base[i];
		i++;
	}
	while (read[j])
	{
		line[i] = read[j];
		i++;
		j++;
	}
	line[i] = '\0';
	//free(base);
	return (line);
}

char	*join(char *base, char *read)
{
	char	*new_read;

	if (!read)
	{
		read = malloc(sizeof(char));
		if (!read)
			return (NULL);
		read[0] = '\0';
	}
	new_read = ft_strjoin_m(base, read);
	//free(read); // PENSER
	return (new_read);
}