/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 12:32:34 by ade-beta          #+#    #+#             */
/*   Updated: 2022/09/01 22:16:06 by eleotard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef enum s_type
{
	word,
	fout,
	fin,
	pip,
	rin,
	rdin,
	rout,
	rdout,
}	t_type;

/*
typedef enum s_type
{
	word,	0 : "tout le reste"
	fout,	1 : >> "this"
	fin,	2 : << "this"
	pip,	3 : |
	rin,	4 : <
	rdin,	5 : <<
	rout,	6 : >
	rdout,	7 : >>
}	t_type;
*/

typedef struct s_token
{
	int				type;
	char			*str;
	int				fd;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	t_token			*arg;
	t_token			*redir;
	int				fdin;
	int				fdout;
	int				pid;
	int				exit;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_env
{
	char			*name;
	char			*content;
	struct s_env	*next;
}	t_env;


void	loop(void);

void	get_word_index(char *s, int *i, int *start);
void	split_words(char *s, int i, int start);
void	tokenize(t_token *token);
void	tokenizing(t_token *token);
void	create_cmd(t_token *token);

t_env	*handler(int opt, char **env, char *name, char *val);

void	print_token(t_token *token);
t_token	*new_token(t_token *next, char *str, int type);
t_token	*token_syntax(t_token *token);

int		quot_status(char *s, int i);
void	skip_quot(char *s, int *i);
char	*del_unused_quot(char *s);
void	del_unquot_extra(char *s, int *i, int *j, char quot);
void	get_type(t_token *tmp, int *f_in, int *f_out);

t_env	*init_env(t_env *next, char *name, char *content);
t_cmd	*init_cmd(t_cmd *next, t_token *arg, t_token *redir);
t_cmd	*cmd_init(t_cmd *res, t_token **tmp, t_token *token);

char	*get_name(char *env);
char	*get_content(char *env);

char	*expand_special(char *tmp, char *util, int *j);
char	*expand_extra(char *tmp, char *util, int *j, int i);
char	*expend_words(char *s, int i);
char	*replace_str(char *tmp, char *util, int j, int i);

void	print_cmd(t_cmd *cmd);
t_token	*cmd_redir(t_token **tmp);
t_token	*cmd_arg(t_token **tmp);
void	add_cmd(t_token **tmp, t_cmd *data);
t_cmd	*pars_err(t_cmd *cmd);

/*FREE*/
void	free_token(t_token *token);
void	free_cmd(t_cmd *cmd);
void	free_env(t_env *env);
void	ctfree(void *ptr, char *err, char type, int n);
void	exit_free(void *ptr, char *err, char type, int n);

/*FREE_UTILS*/
void	*free_tabtab(char **tab);

/*EXEC*/
void    *parent(t_cmd *cmd);



/*UTILS*/
char	*ft_strjoin_m(char *base, char *read);
char	*join(char *base, char *read);

/*TEST PRINTS*/
void	print_tabtab(char **tab);

#endif