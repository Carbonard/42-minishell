/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_parser.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 16:54:26 by nyxssa            #+#    #+#             */
/*   Updated: 2026/02/15 19:20:46 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

// This function assumes that the closing parenthesis exists
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

// This function assumes that the opening parenthesis exists
int	find_open_par(char *str)
{
	int	parenthesis_cnt;
	int	i;

	parenthesis_cnt = -1;
	i = 0;
	while (str[i] && parenthesis_cnt != 0)
	{
		i--;
		if (str[i] == '(')
			parenthesis_cnt++;
		else if (str[i] == ')')
			parenthesis_cnt--;
	}
	return (i);
}

static int	remove_parenthesis(char *str)
{
	int	parenthesis_checker_i;
	int	space_cnt;
	int	removed;

	removed = 0;
	while (*str == ' ' || *str == '(')
	{
		space_cnt = 1;
		if (*str == '(')
		{
			parenthesis_checker_i = find_closing_par(str);
			while (str[parenthesis_checker_i + space_cnt] == ' ')
				space_cnt++;
			if (str[parenthesis_checker_i + space_cnt] == 0)
			{
				str[0] = ' ';
				str[parenthesis_checker_i] = ' ';
				removed = 1;
			}
		}
		str++;
	}
	return (removed);
}

static int
	divide_by_logic_op(t_command_tree *input, t_command_tree **first, t_command_tree **second)
{
	int			i;

	i = ft_strlen(input->cmd) - 1;
	while (i > 0)
	{
		if (input->cmd[i] == ')')
			i += find_open_par(input->cmd + i);
		else if ((input->cmd[i] == '&' && input->cmd[i - 1] == '&')
			|| (input->cmd[i] == '|' && input->cmd[i - 1] == '|'))
		{
			if (input->cmd[i] == '&')
				input->sep = AND;
			else
				input->sep = OR;
			input->cmd[i - 1] = 0;
			*first = malloc(sizeof(t_command_tree));
			*second = malloc(sizeof(t_command_tree));
			(*first)->cmd = input->cmd;
			(*second)->cmd = input->cmd + i + 1;
			input->cmd1 = *first;
			input->cmd2 = *second;
			return (1);
		}
		i--;
	}
	return (0);
}

static int
	divide_by_pipes(t_command_tree *input, t_command_tree **first, t_command_tree **second)
{
	int			i;

	i = ft_strlen(input->cmd) - 1;
	while (i > 0)
	{
		if (input->cmd[i] == ')')
			i += find_open_par(input->cmd + i);
		else if (input->cmd[i] == '|')
		{
			input->sep = PIPE;
			input->cmd[i] = 0;
			*first = malloc(sizeof(t_command_tree));
			*second = malloc(sizeof(t_command_tree));
			(*first)->cmd = input->cmd;
			(*second)->cmd = input->cmd + i + 1;
			input->cmd1 = *first;
			input->cmd2 = *second;
			return (1);
		}
		i--;
	}
	return (0);
}

void	create_tree(t_command_tree *input)
{
	t_command_tree	*first;
	t_command_tree	*second;

	first = NULL;
	second = NULL;
	input->sep = NONE;
	input->subshell = remove_parenthesis(input->cmd);
	if (!divide_by_logic_op(input, &first, &second))
		divide_by_pipes(input, &first, &second);
	input->cmd1 = first;
	input->cmd2 = second;
	if (first)
	{
		create_tree(first);
		create_tree(second);
	}
}
