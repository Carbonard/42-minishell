/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_split_cmd.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 17:03:15 by rselva-2          #+#    #+#             */
/*   Updated: 2026/03/28 23:35:35 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

// static void	cut_string(char *cmd)
// {
// 	char		quote;
// 	int			shift;

// 	shift = 0;
// 	quote = 0;
// 	while (*cmd)
// 	{
// 		if (!quote && (*cmd == '\'' || *cmd == '"'))
// 		{
// 			quote = *cmd;
// 			shift++;
// 		}
// 		else if (quote || (*cmd != ' ' && *cmd != '\n'))
// 		{
// 			*(cmd - shift) = *cmd;
// 			if (*cmd == quote)
// 				shift++;
// 			if (*cmd == quote)
// 				quote = 0;
// 		}
// 		else
// 			*(cmd - shift) = 0;
// 		cmd++;
// 	}
// 	ft_bzero(cmd - shift, shift);
// }

static void	remove_quotes(char *cmd)
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

static int	get_redirection(char *cmd, int i, t_redirection *redir)
{
	int	original_i;

	original_i = i;
	if (cmd[i] == '<' && cmd [i + 1] == '<')
		redir->type_in = HERE_DOC;
	else if (cmd[i] == '>' && cmd [i + 1] == '>')
		redir->type_out = REDIRECTION_APP;
	else if (cmd[i] == '<')
		redir->type_in = REDIRECTION_IN;
	else if (cmd[i] == '>')
		redir->type_out = REDIRECTION_OUT;
	while (cmd[i] == '<' || cmd[i] == '>' || cmd[i] == ' ')
		i++;
	// start = i;
	// while (cmd[i] && cmd[i] != ' ' && !is_redirection(cmd + i))
	// 	i++;
	if (cmd[original_i] == '<')
	{
		redir->file_in = ft_strdup(cmd + i);//ft_substr(cmd, start, i - start);
		remove_quotes(redir->file_in);
	}
	else
	{
		redir->file_out = ft_strdup(cmd + i);//ft_substr(cmd, start, i - start);
		remove_quotes(redir->file_out);
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
			i++;
		return (i);
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
	redir->type_in = NONE;
	redir->type_out = NONE;
	i = 0;
	while (cmd[i])
	{
		while (cmd[i] == ' ')
			i++;
		if (!cmd[i])
			break;
		token = get_token(cmd + i);
		i += ft_strlen(token);
		if (is_redirection(token))
		{
			get_redirection(token, 0, redir);
			free(token);
		}
		else
		{
			remove_quotes(token);
			// printf("quotes removed: '%s'\n", token);
			add_ptr(&split, token);
		}
	}
	add_ptr(&split, NULL);
	return (split.arr);
}

// char	**split_cmd(char *cmd, t_redirection *redir)
// {
// 	t_dyn_ptr	split;
// 	int			length;
// 	int			i;

// 	init_dyn_ptr(&split, 2);
// 	redir->type_in = NONE;
// 	redir->type_out = NONE;
// 	length = ft_strlen(cmd);
// 	cut_string(cmd);
// 	i = 0;
// 	while (i < length)
// 	{
// 		if ((cmd[i] && (i == 0 || cmd[i - 1] == 0)) || cmd[i] == '<' || cmd[i] == '>')
// 		{
// 			if (cmd[i] == '<' || cmd[i] == '>')
// 				i = get_redirection(cmd, i, redir);
// 			else
// 				add_ptr(&split, ft_strdup(cmd + i));
// 		}
// 		i++;
// 	}
// 	add_ptr(&split, NULL);
// 	return (split.arr);
// }
