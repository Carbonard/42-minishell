/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_parser.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 16:54:26 by nyxssa            #+#    #+#             */
/*   Updated: 2026/04/17 16:40:50 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_execution.h"

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

static int	find_open_quote(char *str, int i)
{
	char	quote;

	quote = str[i];
	i--;
	while (i && str[i] != quote)
		i--;
	return (i);
}

int	skip_redirections(char *str, int i)
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

static int	remove_parenthesis(char *str, char **redirections)
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
			if (ft_strchr(str + parenthesis_checker_i + 1, '<')
				|| ft_strchr(str + parenthesis_checker_i + 1, '>'))
				*redirections = ft_strdup(str + parenthesis_checker_i + 1);
			else
				*redirections = NULL;
		}
	}
	return (removed);
}

static int	reduce_index(char *input, int i)
{
	if (input[i] == ')')
		i = find_open_par(input, i);
	else if (input[i] == '\'' || input[i] == '"')
		i = find_open_quote(input, i);
	return (i - 1);
}

static int	divide_by_logic_op(t_command_tree *input, t_command_tree **first, t_command_tree **second)
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

static int	divide_by_pipes(t_command_tree *input, t_command_tree **first, t_command_tree **second)
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
