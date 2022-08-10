/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 14:38:47 by elpastor          #+#    #+#             */
/*   Updated: 2021/11/29 17:27:04 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *s, const char *to_find, size_t n)
{
	size_t	i;
	size_t	j;

	if (!to_find || *to_find == '\0' || s == to_find)
		return ((char *)s);
	i = -1;
	while (++i < n && s[i])
	{
		j = 0;
		while (s[i + j] == to_find[j] && i + j < n)
		{
			if (to_find[j + 1] == 0)
				return ((char *)&s[i]);
			j++;
		}
	}
	return (NULL);
}
