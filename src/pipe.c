/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 17:28:58 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/06 19:17:16 by eleotard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	exec(t_cmd *cmd)
// {
// 	if (!cmd || !cmd->arg || !cmd->arg->str)
// 		exit_free(cmd, NULL, c, 1);
// 	else if (!ft_strcmp(cmd->arg->str, "echo"))
// 		exec_echo(cmd);
// 	else if (!ft_strcmp(cmd->arg->str, "cd"))
// 		exec_cd(cmd);
// 	else if (!ft_strcmp(cmd->arg->str, "pwd"))
// 		exec_pwd(cmd);
// 	else if (!ft_strcmp(cmd->arg->str, "export"))
// 		exec_export(cmd);
// 	else if (!ft_strcmp(cmd->arg->str, "unset"))
// 		exec_unset(cmd);
// 	else if (!ft_strcmp(cmd->arg->str, "env"))
// 		exec_env(cmd);
// 	else if (!ft_strcmp(cmd->arg->str, "exit"))
// 		exec_exit(cmd);
// 	else
// 		exe_cmd(cmd);
// }

/*void	parent(t_cmd *cmd)
{
	if (!get_nbpipe(cmd) && is_built(cmd))
	{
		print_cmd(cmd);
		// exec(cmd);
	}
}*/

void	ft_pipe(t_cmd *cmd)
{
	int	i;
	int	j;
	int	pid;
	//int lst_size;
	int	fd[2][2];
	t_cmd	*tmp;

	i = 1;
	j = 0;
	//lst_size = ft_lstsize(cmd);
	tmp = cmd;
	while (tmp)
	{
		if (i == 1)
			i = 0;
		else if (i == 0)
			i = 1;
		
		if (pipe(fd[i]) < 0)
			return ;
		if (tmp->next)
		{
			if (tmp->fdout == 1)
				tmp->fdout = fd[i][1];
			if (tmp->next->fdin == 0)
			{
				tmp->next->fdin = fd[i][0];
				printf("fd[i][0] = %d && fd[i][1] = %d\n", fd[i][0], fd[i][1]);
			}
		}
		pid = fork();
		if (pid < 0)
			return ;
		if (pid == 0)
		{
			//sleep(10);
			if (tmp->fdin != 0)
			{
				dup2(tmp->fdin, 0);
				//close(tmp->fdin);
			}
				
			if (tmp->fdout != 1)
			{
				printf("\nJJHGHJDGHDGHDGJK\n");
				dup2(tmp->fdout, 1);
				//close(tmp->fdout);
			}
				
			//sleep(10);
			determine_exe_type(tmp);
		}
		/*else 
		{
		// if (i == 1 && j > 0)
			// {
			// 	close(fd[0][0]);
			// 	close(fd[0][1]);
			// }
			// else if (i == 0 && j > 0)
			// {
			// 	close(fd[1][0]);
			// 	close(fd[1][1]);
			// }
			//waitpid(0,0,0);
		}*/
		j++;
		tmp = tmp->next;
	}
	//boucle avec i jusqua la fin de la list et attendre a chque fois chaue commande
	waitpid(0,0,0);
	waitpid(0,0,0);
}