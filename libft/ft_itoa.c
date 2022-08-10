/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 17:09:04 by elpastor          #+#    #+#             */
/*   Updated: 2021/11/30 15:34:40 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	ft_norm(int n, char *s)
{
	if (n < 0)
	{
		s[0] = '-';
		return (-n);
	}
	else
		return (n);
}

static size_t	len_nb(int n)
{
	size_t	count;

	count = 0;
	if (n < 0)
		count++;
	else if (n == 0)
		count++;
	while (n != 0)
	{
		count++;
		n /= 10;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	size_t			i;
	char			*s;
	unsigned int	nb;

	s = (char *)malloc(len_nb(n) + 1);
	if (s == NULL)
		return (NULL);
	nb = ft_norm(n, s);
	i = len_nb(n);
	s[i] = 0;
	if (nb < 10)
	{
		s[--i] = nb + '0';
		nb = 0;
	}
	while (nb != 0)
	{
		s[--i] = (nb % 10) + '0';
		nb /= 10;
	}
	return (s);
}
