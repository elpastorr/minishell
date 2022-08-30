/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 12:48:46 by ade-beta          #+#    #+#             */
/*   Updated: 2022/08/30 22:45:54 by eleotard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	interactif = 1;

void sig_handler(int sig)
{
	if (sig == SIGINT && interactif == 1)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (sig == SIGINT && interactif == 0)
	{
		
	}

	if (sig == SIGQUIT && interactif == 1)
	{
		ft_putstr_fd("\b\b  \b\b", 1);
	}
}

void    loop(void)
{
	char	*s;

	s = NULL;
	while (1)
	{
		s = readline("\e[1m\e[31m\002""Minishell : ""\001\e[0m\002");
		if (s == NULL)
			exit_free(NULL, "exit", 0, 1); // 
		add_history(s);
		split_words(s, 0, 0);
	}
}

int	main(int ac, char **av, char **env)
{
	t_env	*myenv;
	char	*s;
	struct	sigaction sa;

	(void)ac;
	(void)av;
	
	sa.sa_handler =  &sig_handler;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	
	if (!env || !env[0])
	{
		printf("env NULL exit\n");
		return (1);
	}
	
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
