/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 19:05:58 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/12 20:33:50 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

int	is_metachar(char c)
{
	if (c == '<' || c == '>' || c == '|' || c == '&' || c == ' ' || c == '\t'
		|| c == '\n' || c == '(' || c == ')' || c == ';')
		return (1);
	return (0);
}

int	is_redirection(char *str)
{
	if ((str[0] == '<' && str[1] == '<') || (str[0] == '>' && str[1] == '>'))
		return (2);
	if (str[0] == '<' || str[0] == '>')
		return (1);
	return (0);
}

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
		if (str[i])
			return (i + 1);
		else
			return (i);
	}
	return (1);
}
