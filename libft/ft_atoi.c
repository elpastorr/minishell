/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 14:53:40 by elpastor          #+#    #+#             */
/*   Updated: 2021/12/02 17:16:52 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nbr)
{
	int		nb;
	int		neg;
	size_t	i;

	i = 0;
	nb = 0;
	neg = 1;
	while ((nbr[i] >= 9 && nbr[i] <= 13) || nbr[i] == 32)
		i++;
	if (nbr[i] == '+' || nbr[i] == '-')
	{
		if (nbr[i] == '-')
			neg = -1;
		i++;
	}
	while (nbr[i] >= '0' && nbr[i] <= '9')
		nb = (nb * 10 + (nbr[i++] - '0'));
	return (nb * neg);
}
