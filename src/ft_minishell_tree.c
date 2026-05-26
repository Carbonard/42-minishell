/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_tree.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 16:54:26 by nyxssa            #+#    #+#             */
/*   Updated: 2026/05/26 01:49:26 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_execution.h"

// int	reduce_index(char *input, int i)
// {
// 	if (input[i] == ')')
// 		i = find_open_par(input, i);
// 	else if (input[i] == '\'' || input[i] == '"')
// 		i = find_open_quote(input, i);
// 	return (i - 1);
// }

// static int	divide_by_logic_op(
// 	t_command_tree *input, t_command_tree **first, t_command_tree **second)
// {
// 	int	i;

// 	i = ft_strlen(input->cmd);
// 	while (i > 0)
// 	{
// 		i = reduce_index(input->cmd, i);
// 		if (i < 0)
// 			break ;
// 		if ((input->cmd[i] == '&' && input->cmd[i - 1] == '&')
// 			|| (input->cmd[i] == '|' && input->cmd[i - 1] == '|'))
// 		{
// 			if (input->cmd[i] == '&')
// 				input->sep = AND;
// 			else
// 				input->sep = OR;
// 			input->cmd[i - 1] = 0;
// 			*first = malloc(sizeof(t_command_tree));
// 			*second = malloc(sizeof(t_command_tree));
// 			(*first)->cmd = input->cmd;
// 			(*second)->cmd = input->cmd + i + 1;
// 			return (1);
// 		}
// 	}
// 	return (0);
// }

// static int	divide_by_pipes(
// 	t_command_tree *input, t_command_tree **first, t_command_tree **second)
// {
// 	int	i;

// 	i = ft_strlen(input->cmd);
// 	while (i > 0)
// 	{
// 		i = reduce_index(input->cmd, i);
// 		if (i < 0)
// 			break ;
// 		if (input->cmd[i] == '|')
// 		{
// 			input->sep = PIPE;
// 			input->cmd[i] = 0;
// 			*first = malloc(sizeof(t_command_tree));
// 			*second = malloc(sizeof(t_command_tree));
// 			(*first)->cmd = input->cmd;
// 			(*second)->cmd = input->cmd + i + 1;
// 			return (1);
// 		}
// 	}
// 	return (0);
// }

t_str_list	*find_last(t_str_list *token)
{
	while (token && token->next)
		token = token->next;
	return (token);
}

void	ft_str_lstbreak(t_str_list *item)
{
	if (!item)
		return ;
	// fprintf(stderr, "breaking by '%s' previous to '%s'\n", item->content, item->next? item->next->content : "-");
	if (item->prev)
		item->prev->next = NULL;
	if (item->next)
		item->next->prev = NULL;
	free(item->content);
	free(item);
	// // fprintf(stderr, "breacking by '%s'\n", item->content);
	// if (*lst == item)
	// {
	// 	*lst = item->next;
	// 	free(item->content);
	// 	free(item);
	// 	(*lst)->prev = NULL;
	// 	return ;
	// }
	// lst_aux = *lst;
	// while (lst_aux->next && lst_aux->next != item)
	// 	lst_aux = lst_aux->next;
	// if (!lst_aux->next)
	// 	return ;
	// lst_aux->next = NULL;
	// if (item->next)
	// 	item->next->prev = NULL;
	// free(item->content);
	// free(item);
}

static int	divide_by_logic_op(
	t_command_tree *input, t_str_list **first, t_str_list **second)
{
	t_str_list	*token;
	int			par_cnt;

	token = find_last(input->cmd_tokens);
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

	token = find_last(input->cmd_tokens);
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
	if (token)
	{
		*second = token->next;
		ft_str_lstbreak(token);
	}
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
	if (!divide_by_logic_op(input, &first, &second))
		if (!divide_by_pipes(input, &first, &second))
			parse_subshell(input, &first, &second);
	create_subnodes(input, first, second);
}

// void	create_tree(t_command_tree *input)
// {
// 	t_command_tree	*first;
// 	t_command_tree	*second;

// 	// init_dyn_int(&(input->redir.type_in), 0);
// 	// init_dyn_int(&(input->redir.type_out), 0);
// 	// init_dyn_ptr(&(input->redir.file_in), 0);
// 	// init_dyn_ptr(&(input->redir.file_out), 0);
// 	first = NULL;
// 	second = NULL;
// 	input->sep = NONE;
// 	// input->redirections = NULL;
// 	input->subshell = remove_parenthesis(input->cmd);
// 	if (!divide_by_logic_op(input, &first, &second))
// 		divide_by_pipes(input, &first, &second);
// 	input->cmd1 = first;
// 	input->cmd2 = second;
// 	if (first)
// 	{
// 		create_tree(first);
// 		create_tree(second);
// 	}
// }
