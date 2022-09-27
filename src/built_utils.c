/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 15:03:24 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/23 13:43:29 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	ex_cd_plus(t_env *env, char **s, int *f)
{
	if (env && (!(*s) || !ft_strcmp(*s, "~")))
	{
		*s = ft_strdup(env->content);
		*f = 1;
	}
	if (env && *s && (*s)[0] == '~' && (*s)[1])
	{
		*s = ft_strjoin(env->content, &(*s)[1]);
		*f = 1;
	}
}

long long int	exit_atoi(char *s, int *err)
{
	int	i;
	int	neg;

	if (!s)
		return (0);
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
	return (exit_atoi_plus(s, i, neg, err));
}

long long int	exit_atoi_plus(char *s, int i, int neg, int *err)
{
	unsigned long long int	nb;

	nb = 0;
	while (s[i] && ft_isdigit(s[i]))
		nb = nb * 10 + s[i++] - '0';
	while (s[i] && ft_isspace(s[i]))
		i++;
	if (s[i])
		*err = 2;
	if ((neg == 1 && nb > 9223372036854775807)
		|| (neg == -1 && nb > 9223372036854775808u))
		*err = 2;
	if (neg == 1)
		return ((nb * neg) % 256);
	nb %= 256;
	nb -= 256;
	return (nb * neg);
}

int	get_equal2(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	if (!ft_isalpha(s[i]) && s[i++] != '_')
		return (0);
	while (s[i] && (ft_isalnum(s[i]) || s[i] == '='
			|| s[i] == '+' || s[i] == '_'))
	{
		if (s[i] == '+')
			return (i);
		i++;
	}
	return (0);
}
