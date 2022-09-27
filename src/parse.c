/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 16:53:21 by elpastor          #+#    #+#             */
/*   Updated: 2022/09/24 19:05:41 by eleotard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_word_index(char *s, int *i, int *start)
{
	while (s[*i] == ' ' || s[*i] == '\t')
		(*i)++;
	*start = *i;
	if (!ft_strncmp(&s[*i], "<<", 2) || !ft_strncmp(&s[*i], ">>", 2))
		(*i) += 2;
	else if (s[*i] == '<' || s[*i] == '>' || s[*i] == '|')
		(*i)++;
	else
	{
		while (s[*i] && s[*i] != '|' && s[*i] != '<' && s[*i] != '>'
			&& s[*i] != ' ' && s[*i] != '\t')
		{
			if (s[*i] == '"' || s[*i] == '\'')
				skip_quot(s, i);
			else
				(*i)++;
		}
	}
}

void	split_words(char *s, int i, int start)
{
	t_token	*token;
	t_token	*tmp;

	if (!s)
		return ;
	if (!s[0])
		return (free(s));
	get_word_index(s, &i, &start);
	token = new_token(NULL, strndup(&s[start], i - start), 0);
	tmp = token;
	while (s[i])
	{
		get_word_index(s, &i, &start);
		if (i > start)
		{
			token->next = new_token(NULL, strndup(&s[start], i - start), 0);
			token = token->next;
			if (!token)
				ctfree(tmp, "error init token...", 't', 1);
		}
	}
	ft_memdel((void **)&s);
	tokenize(tmp);
}

void	tokenize(t_token *token)
{
	t_token	*tmp;
	int		f_in;
	int		f_out;

	tmp = token;
	f_in = 0;
	f_out = 0;
	while (tmp)
	{
		get_type(tmp, &f_in, &f_out);
		if (tmp->type == rdout || tmp->type == rout || tmp->type == rin)
			f_in = 1;
		if (tmp->type == rdin)
			f_out = 1;
		tmp = tmp->next;
	}
	if (!token_syntax(token))
		return ;
	tokenizing(token);
}

void	tokenizing(t_token *token)
{
	t_token	*tmp;
	int		i;

	tmp = token;
	while (tmp)
	{
		if (tmp->type == word || tmp->type == fout)
		{
			i = 0;
			while (tmp->str && tmp->str[i])
			{
				if (tmp->str[i] == '$' && quot_status(tmp->str, i) != 1
					&& ft_isalnumplus(tmp->str[i + 1]))
					tmp->str = expend_words(tmp->str, i);
				if (tmp->str[i] != '$' || quot_status(tmp->str, i) == 1)
					i++;
				if (i > (int)ft_strlen(tmp->str))
					break ;
			}
		}
		tmp = tmp->next;
	}
	if (!token)
		return ;
	create_cmd(del_unused_quot_plus(token));
}

void	create_cmd(t_token *token)
{
	t_cmd	*data;
	t_cmd	*temp;
	t_token	*tmp;

	data = init_cmd(NULL, NULL, NULL);
	if (!data)
		exit_free(token, "Error cmd init...", 't', 1);
	temp = data;
	tmp = token;
	while (tmp)
	{
		if (tmp->type == pip)
		{
			data->next = cmd_init(temp, &tmp, token);
			data = data->next;
		}
		else
			add_cmd(&tmp, data);
	}
	if (!temp || !pars_err(temp))
		return ;
	if (!redir(temp) && get_cmd_size(temp) == 1)
		return (ctfree(temp, NULL, 'c', 1));
	parent(temp, 0);
}
