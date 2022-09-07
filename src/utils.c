/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 17:23:56 by eleotard          #+#    #+#             */
/*   Updated: 2022/09/07 18:53:39 by elpastor         ###   ########.fr       */
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
	line = malloc(sizeof(char) * (ft_strlen(base) + ft_strlen(read) + 1));
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

void	ft_free_opt(char *s1, char *s2, int opt)
{
	if (opt == 0)
		free(s1);
	else if (opt == 1)
		free(s2);
	else if (opt == 2)
	{
		free(s1);
		free(s2);
	}
}

char	*ft_strjoin_free(char *s1, char *s2, int opt)
{
	int		i;
	int		j;
	char	*dst;

	if (!s1 || !s2)
		return (NULL);
	dst = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (dst == NULL)
		return (NULL);
	i = -1;
	while (s1[++i])
		dst[i] = s1[i];
	j = -1;
	while (s2[++j])
		dst[i + j] = s2[j];
	dst[i + j] = 0;
	ft_free_opt(s1, s2, opt);
	return (dst);
}
