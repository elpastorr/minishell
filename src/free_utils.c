/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 17:33:22 by eleotard          #+#    #+#             */
/*   Updated: 2022/09/01 19:09:23 by eleotard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*free_tabtab(char **tab)
{
	int	k;

	k = 0;
	while (tab[k])
		k++;
	while (k > 0)
	{
		free(tab[k]);
		k--;
	}
	free(tab);
	return (NULL);
}