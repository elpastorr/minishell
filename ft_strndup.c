/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 17:09:37 by elpastor          #+#    #+#             */
/*   Updated: 2022/08/10 17:17:29 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, int n)
{
	char	*str;
	int		i;

	str = (char *)malloc(n + 1);
	if (str == NULL)
		return (0);
	i = -1;
	while (s[++i] && i < n)
		str[i] = s[i];
	str[i] = 0;
	return (str);
}
