/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_split_cmd.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 17:03:15 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/18 20:37:21 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

static void	cut_string(char *cmd)
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
		else if (quote || *cmd != ' ')
		{
			*(cmd - shift) = *cmd;
			if (*cmd == quote)
				shift++;
			if (*cmd == quote)
				quote = 0;
		}
		else
			*(cmd - shift) = 0;
		cmd++;
	}
	ft_bzero(cmd - shift, shift);
}

static int	get_redirection(char *cmd, int i, t_redirection *redir)
{
	if (cmd[i] == '<' && cmd [i + 1] == '<')
		redir->type = HERE_DOC;
	else if (cmd[i] == '>' && cmd [i + 1] == '>')
		redir->type = REDIRECTION_APP;
	else if (cmd[i] == '<')
		redir->type = REDIRECTION_IN;
	else if (cmd[i] == '>')
		redir->type = REDIRECTION_OUT;
	while (cmd[i] == '<' || cmd[i] == '>' || cmd[i] == 0)
		i++;
	redir->file = cmd + i;
	return (i);
}

char	**split_cmd(char *cmd, t_redirection *redir)
{
	t_dyn_ptr	split;
	int			length;
	int			i;

	init_dyn_ptr(&split, 2);
	length = ft_strlen(cmd);
	cut_string(cmd);
	i = 0;
	while (i < length)
	{
		if (cmd[i] && (i == 0 || cmd[i - 1] == 0))
		{
			if (cmd[i] == '<' || cmd[i] == '>')
				i = get_redirection(cmd, i, redir);
			else
				add_ptr(&split, cmd + i);
		}
		i++;
	}
	add_ptr(&split, NULL);
	return (split.arr);
}
