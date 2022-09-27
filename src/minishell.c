/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 12:48:46 by ade-beta          #+#    #+#             */
/*   Updated: 2022/09/23 19:33:54 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	only_space(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != '\f' && str[i] != '\t' && str[i] != '\n'
			&& str[i] != '\r' && str[i] != '\v' && str[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}

void	loop(void)
{
	char	*s;

	s = NULL;
	while (1)
	{
		s = readline("\e[1m\e[31m\002""Minishell : ""\001\e[0m\002");
		if (s == NULL)
		{
			ft_putstr_fd("exit\n", 2);
			exit_free(NULL, NULL, 0, 1);
		}
		add_history(s);
		if (only_space(s))
			continue ;
		split_words(s, 0, 0);
	}
}

int	main(int ac, char **av, char **env)
{
	t_env	*myenv;
	char	*s;

	(void)ac;
	(void)av;
	catch_signals();
	handler(0, env, NULL, NULL);
	myenv = handler(3, NULL, "SHLVL", NULL);
	if (!myenv)
		myenv = handler(3, NULL, "SHLVL", "1");
	s = ft_itoa(ft_atoi(myenv->content) + 1);
	handler(3, NULL, "SHLVL", s);
	free(s);
	loop();
	return (1);
}
