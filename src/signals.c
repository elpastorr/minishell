/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/04 15:36:25 by eleotard          #+#    #+#             */
/*   Updated: 2022/09/20 15:46:00 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>

void	catch_sigint(int sig)
{
	(void)sig;
	if (write(1, "\n", 1) == -1)
		return ;
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	catch_sigquit(int sig)
{
	(void)sig;
	ft_putstr_fd("\b\b  \b\b", 1);
}

void	reset_default_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTSTP, SIG_DFL);
	
	/*struct sigaction	sigint;
	struct sigaction	sigquit;

	sigint.sa_handler = SIG_DFL;
	sigint.sa_flags = 0;
	sigquit.sa_handler = SIG_DFL;
	sigquit.sa_flags = 0;
	sigaction(SIGINT, &sigint, NULL);
	sigaction(SIGQUIT, &sigquit, NULL);
	//signal(SIGINT)
	signal(SIGTSTP, SIG_DFL);*/
}

void	catch_signals(void)
{
	signal(SIGINT, catch_sigint);
	signal(SIGQUIT, catch_sigquit);
	
	/*struct sigaction	sigint;
	struct sigaction	sigquit;

	sigint.sa_handler = catch_sigint;
	sigint.sa_flags = SA_RESTART;
	sigquit.sa_handler = catch_sigquit;
	sigquit.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sigint, NULL);
	sigaction(SIGQUIT, &sigquit, NULL);
	signal(SIGTSTP, SIG_IGN);*/
}

void	here_handler_sigint(int sig)
{
	(void)sig;
	close(0);
}
