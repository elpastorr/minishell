/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 12:32:34 by ade-beta          #+#    #+#             */
/*   Updated: 2022/09/16 14:46:21 by eleotard         ###   ########.fr       */
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
# include <errno.h>

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

/*PARSE*/
void	get_word_index(char *s, int *i, int *start);
void	split_words(char *s, int i, int start);
void	tokenize(t_token *token);
void	tokenizing(t_token *token);
void	create_cmd(t_token *token);

/*HANDLER*/
t_env	*handler(int opt, char **env, char *name, char *val);

/*HEREDOC*/
int		is_heredoc(t_cmd *cmd);
char	*heredoc_strcat(char *tmp, char *s);
char	*read_heredoc(char *s, char *tmp);
char	*heredoc_extra(t_token *redir, char *tmp, int ret);
char	*heredoc(t_cmd *cmd);

/*REDIR*/
void	print_err(char *file, char *s);
int		file_err(t_token *tmp);
t_cmd	*redir(t_cmd *cmd);

/*TOKEN*/
void	print_token(t_token *token);
t_token	*new_token(t_token *next, char *str, int type);
t_token	*token_syntax(t_token *token);

/*QUOTE*/
int		quot_status(char *s, int i);
void	skip_quot(char *s, int *i);
char	*del_unused_quot(char *s);
void	del_unquot_extra(char *s, int *i, int *j, char quot);
void	get_type(t_token *tmp, int *f_in, int *f_out);

/*INIT*/
t_env	*init_env(t_env *next, char *name, char *content);
t_cmd	*init_cmd(t_cmd *next, t_token *arg, t_token *redir);
t_cmd	*cmd_init(t_cmd *res, t_token **tmp, t_token *token);

/*GET*/
char	*get_name(char *env);
char	*get_content(char *env);
int		get_nbpipe(t_cmd *cmd);

/*EXPAND*/
char	*expand_special(char *tmp, char *util, int *j);
char	*expand_extra(char *tmp, char *util, int *j, int i);
char	*expend_words(char *s, int i);
char	*replace_str(char *tmp, char *util, int j, int i);

/*CMD*/
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
void	free_tabs_exit_free(t_cmd *cmd, char **env, char **argv, char *err);

/*EXEC*/
void    *parent(t_cmd *cmd);
void	determine_exe_type(t_cmd *cmd);

char	**create_env_tab(t_env *env, int nb_of_lines);
char	**get_exec_env(void);
char	**get_exec_args(t_cmd *cmd, int nb_of_arg);
int		find_slash(t_cmd *cmd);

int		find__args(t_cmd *cmd);

void	exec(t_cmd *cmd, const char *pathname);
void	exec_cmd_without_redir(t_cmd *cmd, const char *pathname, int nb_of_arg, char **env);

/*SIGNALS*/
void	catch_signals(void);

/*PATHS*/
char	*find_path(t_cmd *cmd, char **tab_of_paths);
char	*look_for_path(t_cmd *cmd);

/*UTILS*/
char	*ft_strjoin_m(char *base, char *read);
char	*join(char *base, char *read);

/*TEST PRINTS*/
void	print_tabtab(char **tab);

void	ft_multi_pipe(t_cmd *cmd);



#endif