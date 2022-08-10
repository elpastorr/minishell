/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 15:21:47 by elpastor          #+#    #+#             */
/*   Updated: 2021/11/22 15:23:16 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	lendst;

	i = -1;
	lendst = ft_strlen(dst);
	if (size == 0 || lendst >= size)
		return (size + ft_strlen((char *)src));
	while (src[++i] && (i + lendst < size - 1))
		dst[lendst + i] = src[i];
	dst[lendst + i] = 0;
	return (lendst + ft_strlen((char *)src));
}
