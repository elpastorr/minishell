/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 15:03:24 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/14 16:01:32 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_isspace(char c)
{
	if ((c >= '\t' && c <= '\r') || c == ' ')
		return (1);
	return (0);
}

int	only_n(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

long long int	exit_atoi(char *s, int *err)
{
	unsigned long long int  nb;
	int i;
	int neg;

	if (!s)
		return (0);
	nb = 0;
	i = 0;
	neg = 1;
	while (s[i] && ft_isspace(s[i]))
		i++;
	if (s[i] == '-' || s[i] == '+')
	{
		if (s[i] == '-')
			neg = -1;
		i++;
	}
	if (!ft_isdigit(s[i]))
		*err = 2;
	while (s[i] && ft_isdigit(s[i]))
		nb = nb * 10 + s[i++] - '0';
	while (s[i] && ft_isspace(s[i]))
		i++;
	return (exit_atoi_plus(s, i, nb, neg, err));
}

long long int   exit_atoi_plus(char *s, int i, unsigned long long int nb, int neg, int *err)
{
	if (s[i])
		*err = 2;
	if ((neg == 1 && nb > 9223372036854775807) || (neg == -1 && nb > 9223372036854775808u))
		*err = 2;
	if (neg == 1)
		return ((nb * neg) % 256);
	nb %= 256;
	nb -= 256;
	return (nb * neg);
}
