/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_test.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 22:13:56 by eleotard          #+#    #+#             */
/*   Updated: 2022/09/01 23:01:17 by eleotard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tabtab(char **tab)
{
	int	k;

	k = 0;
	if (!tab)
	{
		printf("Pas de tabtab a print\n");
		return ;
	}
	while (tab[k])
	{
		printf("et la ?\n");
		printf("[%s]\n", tab[k]);
		fflush(stdout);
		k++;
	}
	printf("est passe la ?\n");
}