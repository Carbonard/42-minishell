/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_split_cmd.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 17:03:15 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/19 01:51:35 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_execution.h"

void	remove_quotes(char *cmd)
{
	char		quote;
	int			shift;

	shift = 0;
	quote = 0;
	while (*cmd)
	{
		if (!quote && (*cmd == '\'' || *cmd == '"'))
		{
			quote = *cmd;
			shift++;
		}
		else if (quote && *cmd == quote)
			shift++;
		else
			*(cmd - shift) = *cmd;
		cmd++;
	}
	*(cmd - shift) = 0;
}

static int	get_redirection(char *token, int i, t_redirection *redir)
{
	int	original_i;
	int	start;

	original_i = i;
	if (token[i] == '<' && token [i + 1] == '<')
		add_int(&(redir->type_in), HERE_DOC);
	else if (token[i] == '>' && token [i + 1] == '>')
		add_int(&(redir->type_out), REDIRECTION_APP);
	else if (token[i] == '<')
		add_int(&(redir->type_in), REDIRECTION_IN);
	else if (token[i] == '>')
		add_int(&(redir->type_out), REDIRECTION_OUT);
	while (token[i] == '<' || token[i] == '>' || token[i] == ' ')
		i++;
	start = i;
	while (token[i] && !is_metachar(token[i]))
		i++;
	if (token[original_i] == '<')
	{
		if (token[original_i + 1] != '<')
		{
			add_ptr(&(redir->file_in), ft_substr(token, start, i - start));
			remove_quotes(redir->file_in.arr[redir->file_in.length - 1]);
		}
		else
			add_ptr(&(redir->file_in), ft_calloc(1,1));
	}
	else if (token[original_i] == '>')
	{
		add_ptr(&(redir->file_out), ft_substr(token, start, i - start));
		remove_quotes(redir->file_out.arr[redir->file_out.length - 1]);
	}
	return (i);
}

int	advance_quotes(char *str)
{
	char	quote;
	int		i;

	i = 0;
	if (str[i] == '\'' || str[i] == '"')
	{
		quote = str[i];
		i++;
		while (str[i] && str[i] != quote)
		{
			i++;
		}
		return (i + 1);
	}
	return (1);
}

char *get_token(char *cmd)
{
	int		i;

	i = 0;
	if (is_redirection(cmd))
	{
		while (cmd[i] && (cmd[i] == ' ' || cmd[i] == '>' || cmd[i] == '<'))
			i++;
	}
	while (cmd[i] && cmd[i] != ' ' && !is_redirection(cmd + i))
		i += advance_quotes(cmd + i);
	return (ft_substr(cmd, 0, i));
}

char	**split_cmd(char *cmd, t_redirection *redir)
{
	t_dyn_ptr	split;
	char		*token;
	int			i;

	init_dyn_ptr(&split, 2);
	i = 0;
	while (cmd && cmd[i])
	{
		while (cmd[i] == ' ' || cmd[i] == '\n')
			i++;
		if (!cmd[i])
			break;
		token = get_token(cmd + i);
		i += ft_strlen(token);
		if (is_redirection(token))
			get_redirection(token, 0, redir);
		if (is_redirection(token))
			free(token);
		else
		{
			remove_quotes(token);
			add_ptr(&split, token);
		}
	}
	return (split.arr);
}
