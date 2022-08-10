/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 12:32:34 by ade-beta          #+#    #+#             */
/*   Updated: 2022/08/10 19:36:20 by elpastor         ###   ########.fr       */
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
	fin,	2 : "this" <<
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

char	*env_handle(char **envd, int opt, char *str, char *cont);
char	*get_var(t_env *env, char *str);
t_env	*unset_var(t_env *env, char *str);
void	export_var(t_env *env, char *str, char *cont);
void	set_sig(void);


void	loop(void);

void	split_words(char *s, int i, int start);
void	get_word_index(char *s, int *i, int *start);
t_env	*handler(int opt, char **env, char *name, char *val);

t_token	*new_token(t_token *next, char *str, int type);
int		token_syntax(t_token *token);
void	tokenize(t_token *token);
void	tokenizing(t_token *token);

int		quot_status(char *s, int i);
void	skip_quot(char *s, int *i);
char	*del_unused_quot(char *s);
void	del_unquot_extra(char *s, int *i, int *j, char quot);
void	get_type(t_token *tmp, int *f_in, int *f_out);

t_env	*init_env(t_env *next, char *name, char *content);
static t_env	*init_handler(char **env, int *exit_status);

char	*get_name(char *env);
char	*get_content(char *env);

char	*expand_special(char *tmp, char *util, int *j);
char	*expand_extra(char *tmp, char *util, int *j, int i);
char	*expend_words(char *s, int i);
char	*replace_str(char *tmp, char *util, int j, int i);


#endif