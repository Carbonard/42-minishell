/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_tree.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 16:54:26 by nyxssa            #+#    #+#             */
/*   Updated: 2026/06/06 19:54:12 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_execution.h"

static int	divide_by_logic_op(
	t_command_tree *input, t_str_list **first, t_str_list **second)
{
	t_str_list	*token;
	int			par_cnt;

	token = ft_str_lstlast(input->cmd_tokens);
	par_cnt = 0;
	while (token)
	{
		if (token->content[0] == ')')
			par_cnt++;
		else if (token->content[0] == '(')
			par_cnt--;
		else if (!par_cnt && (!ft_strncmp(token->content, "||", 3)
				|| !ft_strncmp(token->content, "&&", 3)))
		{
			*first = input->cmd_tokens;
			input->cmd_tokens = NULL;
			*second = token->next;
			input->sep = OR * !ft_strncmp(token->content, "||", 3)
				+ AND * !ft_strncmp(token->content, "&&", 3);
			ft_str_lstbreak(token);
			return (1);
		}
		token = token->prev;
	}
	return (0);
}

static int	divide_by_pipes(
	t_command_tree *input, t_str_list **first, t_str_list **second)
{
	t_str_list	*token;
	int			par_cnt;

	token = ft_str_lstlast(input->cmd_tokens);
	par_cnt = 0;
	while (token)
	{
		if (token->content[0] == ')')
			par_cnt++;
		else if (token->content[0] == '(')
			par_cnt--;
		else if (!par_cnt && !ft_strncmp(token->content, "|", 2))
		{
			*first = input->cmd_tokens;
			input->cmd_tokens = NULL;
			*second = token->next;
			input->sep = PIPE;
			ft_str_lstbreak(token);
			return (1);
		}
		token = token->prev;
	}
	return (0);
}

static void	parse_subshell(
	t_command_tree *input, t_str_list **first, t_str_list **second)
{
	t_str_list	*token;
	int			par_cnt;

	if (input->cmd_tokens->content[0] != '(')
		return ;
	ft_str_delitem(&(input->cmd_tokens), input->cmd_tokens);
	token = input->cmd_tokens;
	input->cmd_tokens = NULL;
	*first = token;
	par_cnt = 1;
	while (token->next && par_cnt)
	{
		if (token && token->content[0] == '(')
			par_cnt++;
		if (token && token->content[0] == ')')
			par_cnt--;
		token = token->next;
	}
	if (token->next)
		token = token->prev;
	*second = token->next;
	ft_str_lstbreak(token);
	input->subshell = 1;
}

void	create_subnodes(
	t_command_tree *input, t_str_list *first, t_str_list *second)
{
	if (first)
	{
		input->cmd1 = malloc(sizeof(t_command_tree));
		if (input->cmd1)
		{
			input->cmd1->cmd_tokens = first;
			create_tree(input->cmd1);
		}
	}
	if (second && !input->subshell)
	{
		input->cmd2 = malloc(sizeof(t_command_tree));
		if (input->cmd2)
		{
			input->cmd2->cmd_tokens = second;
			create_tree(input->cmd2);
		}
	}
	if (second && input->subshell)
		input->subshell_redirections = second;
}

void	create_tree(t_command_tree *input)
{
	t_str_list	*first;
	t_str_list	*second;

	first = NULL;
	second = NULL;
	input->cmd1 = NULL;
	input->cmd2 = NULL;
	input->sep = NONE;
	input->subshell = 0;
	input->subshell_redirections = NULL;
	input->hd_fd = -1;
	if (!divide_by_logic_op(input, &first, &second))
		if (!divide_by_pipes(input, &first, &second))
			parse_subshell(input, &first, &second);
	create_subnodes(input, first, second);
}
