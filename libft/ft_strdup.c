/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 11:36:59 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/16 17:17:31 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*str;
	size_t	i;

	str = (char *)malloc(ft_strlen(s) + 1);
	if (str == NULL)
		return (NULL);
	i = -1;
	while (s[++i])
		str[i] = s[i];
	str[i] = 0;
	return (str);
}
