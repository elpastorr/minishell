/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 12:48:46 by ade-beta          #+#    #+#             */
/*   Updated: 2022/08/10 17:02:22 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    loop(void)
{
	char	*s;

	s = NULL;
	while (1)
	{
		s = readline("\e[1m\e[31m\002""Minishell : ""\001\e[0m\002");
		if (s == NULL)
			return ;
		add_history(s);
		split_words(s, 0, 0);
		free(s);
	}
}

int	main(int ac, char **av, char **env)
{
	t_env	*myenv;
	
	(void)ac;
	(void)av;
	if (!env || !env[0])
	{
		printf("env NULL exit\n");
		return (1);
	}
	handler(0, env, NULL, NULL);
	myenv = handler(3, NULL, "SHLVL", NULL);
	(void)myenv;
	loop();
	return (1);
}
