/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_parser.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 16:54:26 by nyxssa            #+#    #+#             */
/*   Updated: 2026/01/20 00:00:43 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

static int	find_closing_par(char *str)
{
	int	parenthesis_checker;
	int	parenthesis_checker_i;

	parenthesis_checker = 1;
	parenthesis_checker_i = 0;
	while (parenthesis_checker != 0)
	{
		parenthesis_checker_i++;
		if (str[parenthesis_checker_i] == '(')
			parenthesis_checker++;
		else if (str[parenthesis_checker_i] == ')')
			parenthesis_checker--;
	}
	return (parenthesis_checker_i);
}

static void	remove_parenthesis(char *str)
{
	int	parenthesis_checker_i;
	int	space_cnt;

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
			}
		}
		str++;
	}
}

static int
	divide_by_logic_op(t_command *input, t_command **first, t_command **second)
{
	int			i;

	i = 0;
	remove_parenthesis(input->cmd);
	while (input->cmd[i] != 0)
	{
		if (input->cmd[i] == '(')
			i += find_closing_par(input->cmd + i);
		else if ((input->cmd[i] == '&' && input->cmd[i + 1] == '&')
			|| (input->cmd[i] == '|' && input->cmd[i + 1] == '|'))
		{
			if (input->cmd[i] == '&')
				input->sep = AND;
			else
				input->sep = OR;
			input->cmd[i] = 0;
			*first = malloc(sizeof(t_command));
			*second = malloc(sizeof(t_command));
			(*first)->cmd = input->cmd;
			(*second)->cmd = input->cmd + i + 2;
// printf("En logic:\nfirst:%s\nsecond:%s\n", (*first)->cmd, (*second)->cmd);
			input->cmd1 = *first;
			input->cmd2 = *second;
			return (1);
		}
		i++;
	}
	return (0);
}

static int
	divide_by_pipes(t_command *input, t_command **first, t_command **second)
{
	int			i;

	i = 0;
	remove_parenthesis(input->cmd);
	while (input->cmd[i] != 0)
	{
		if (input->cmd[i] == '(')
			i += find_closing_par(input->cmd + i);
		else if (input->cmd[i] == '|')
		{
			input->sep = PIPE;
			input->cmd[i] = 0;
			*first = malloc(sizeof(t_command));
			*second = malloc(sizeof(t_command));
			(*first)->cmd = input->cmd;
			(*second)->cmd = input->cmd + i + 1;
// printf("En pipes:\nfirst:%s\nsecond:%s\n", (*first)->cmd, (*second)->cmd);
			input->cmd1 = *first;
			input->cmd2 = *second;
			return (1);
		}
		i++;
	}
	return (0);
}

void	create_tree(t_command *input)
{
	t_command	*first;
	t_command	*second;

	first = NULL;
	second = NULL;
	input->sep = NONE;
	remove_parenthesis(input->cmd);
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
