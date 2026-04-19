/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_tree.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 16:54:26 by nyxssa            #+#    #+#             */
/*   Updated: 2026/04/19 01:37:35 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_execution.h"

int	reduce_index(char *input, int i)
{
	if (input[i] == ')')
		i = find_open_par(input, i);
	else if (input[i] == '\'' || input[i] == '"')
		i = find_open_quote(input, i);
	return (i - 1);
}

static int	divide_by_logic_op(
	t_command_tree *input, t_command_tree **first, t_command_tree **second)
{
	int	i;

	i = ft_strlen(input->cmd);
	while (i > 0)
	{
		i = reduce_index(input->cmd, i);
		if (i < 0)
			break;
		if ((input->cmd[i] == '&' && input->cmd[i - 1] == '&')
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
			return (1);
		}
	}
	return (0);
}

static int	divide_by_pipes(
	t_command_tree *input, t_command_tree **first, t_command_tree **second)
{
	int	i;

	i = ft_strlen(input->cmd);
	while (i > 0)
	{
		i = reduce_index(input->cmd, i);
		if (i < 0)
			break;
		if (input->cmd[i] == '|')
		{
			input->sep = PIPE;
			input->cmd[i] = 0;
			*first = malloc(sizeof(t_command_tree));
			*second = malloc(sizeof(t_command_tree));
			(*first)->cmd = input->cmd;
			(*second)->cmd = input->cmd + i + 1;
			return (1);
		}
	}
	return (0);
}

void	create_tree(t_command_tree *input)
{
	t_command_tree	*first;
	t_command_tree	*second;

	init_dyn_int(&(input->redir.type_in), 0);
	init_dyn_int(&(input->redir.type_out), 0);
	init_dyn_ptr(&(input->redir.file_in), 0);
	init_dyn_ptr(&(input->redir.file_out), 0);
	first = NULL;
	second = NULL;
	input->sep = NONE;
	input->redirections = NULL;
	input->subshell = remove_parenthesis(input->cmd, &(input->redirections));
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
