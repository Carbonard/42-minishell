/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 19:05:58 by rselva-2          #+#    #+#             */
/*   Updated: 2026/06/07 18:39:38 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

char	*remove_quotes(char *cmd)
{
	char		*original_cmd;
	char		quote;
	int			shift;

	original_cmd = cmd;
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
	return (original_cmd);
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
