/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_tree_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 00:21:41 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/23 15:50:59 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_execution.h"

int	find_closing_par(char *str)
{
	int	parenthesis_checker;
	int	parenthesis_checker_i;

	parenthesis_checker = 1;
	parenthesis_checker_i = 0;
	while (str[parenthesis_checker_i] && parenthesis_checker != 0)
	{
		parenthesis_checker_i++;
		if (str[parenthesis_checker_i] == '(')
			parenthesis_checker++;
		else if (str[parenthesis_checker_i] == ')')
			parenthesis_checker--;
	}
	return (parenthesis_checker_i);
}

int	find_open_par(char *str, int i)
{
	int	parenthesis_cnt;

	parenthesis_cnt = -1;
	while (i && parenthesis_cnt != 0)
	{
		i--;
		if (str[i] == '(')
			parenthesis_cnt++;
		else if (str[i] == ')')
			parenthesis_cnt--;
	}
	return (i);
}

int	find_open_quote(char *str, int i)
{
	char	quote;

	quote = str[i];
	i--;
	while (i && str[i] != quote)
		i--;
	return (i);
}

static int	skip_redirections(char *str, int i)
{
	int	redir_found;

	redir_found = 1;
	while (str[i] && redir_found)
	{
		redir_found = 0;
		while (str[i] == ' ')
			i++;
		while (str[i] == '<' || str[i] == '>')
		{
			redir_found = 1;
			i++;
		}
		while (str[i] == ' ')
			i++;
		while (redir_found && str[i] && str[i] != ' ' && str[i] != '<'
			&& str[i] != '>' && str[i] != '&' && str[i] != '|')
			i++;
		if (str[i] == '&' || str[i] == '|')
			return (i);
	}
	return (i);
}

int	remove_parenthesis(char *str)
{
	int		parenthesis_checker_i;
	int		final;
	int		removed;

	removed = 0;
	while (*str == ' ')
		str++;
	if (*str == '(')
	{
		parenthesis_checker_i = find_closing_par(str);
		final = skip_redirections(str, parenthesis_checker_i + 1);
		if (str[final] == 0)
		{
			str[0] = ' ';
			str[parenthesis_checker_i] = 0;
			removed = 1;
			// if (ft_strchr(str + parenthesis_checker_i + 1, '<')
			// 	|| ft_strchr(str + parenthesis_checker_i + 1, '>'))
			// 	*redirections = ft_strdup(str + parenthesis_checker_i + 1);
			// else
			// 	*redirections = NULL;
		}
	}
	return (removed);
}
