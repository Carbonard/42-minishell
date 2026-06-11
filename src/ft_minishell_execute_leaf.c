/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_execute_leaf.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 03:50:34 by rselva-2          #+#    #+#             */
/*   Updated: 2026/06/11 19:35:05 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_execution.h"
#include "ft_minishell_input.h"

static void	expand_tokens(
				t_context *ctx, t_command_tree *node, t_str_list *token)
{
	t_str_list	*new_token;
	t_str_list	*saved_token;

	new_token = get_tokens(ctx, token->content, SPACES);
	while (new_token)
	{
		saved_token = new_token->next;
		ft_str_lst_preinstert(token, new_token);
		new_token = saved_token;
	}
	if (node->cmd_tokens == token)
		while (node->cmd_tokens->prev)
			node->cmd_tokens = node->cmd_tokens->prev;
}

static void	init_argv(t_context *ctx, t_command_tree *node, t_str_list *token)
{
	t_str_list	*saved_token;
	char		*prev;

	ctx->original_in = dup(STDIN_FILENO);
	if (ctx->original_in == -1)
		ctx->original_in = -2;
	ctx->original_out = dup(STDOUT_FILENO);
	if (ctx->original_out == -1)
		ctx->original_out = -2;
	while (token && ctx->status == MS_SUCCESS)
	{
		prev = token->content;
		token->content = expand_variables(ctx, token->content);
		if (!token->content)
			malloc_exit(ctx);
		else if (token->content != prev)
		{
			expand_tokens(ctx, node, token);
			saved_token = token->next;
			ft_str_delitem(&(node->cmd_tokens), token);
			token = saved_token;
		}
		else
			token = token->next;
	}
}

static void	manage_wildcards(t_context *ctx, char *token, t_dyn_ptr *argv)
{
	char	*unquoted_token;
	int		e_status;

	e_status = expand_wildcards(token, argv);
	if (e_status == -1)
	{
		free_dyn_ptr(argv);
		malloc_exit(ctx);
	}
	if (e_status == 0)
	{
		unquoted_token = ft_strdup(token);
		if (!unquoted_token)
		{
			free_dyn_ptr(argv);
			malloc_exit(ctx);
		}
		remove_quotes(unquoted_token);
		if (!add_ptr(argv, unquoted_token))
		{
			free(unquoted_token);
			free_dyn_ptr(argv);
			malloc_exit(ctx);
		}
	}
}

char	**manage_argv_and_redirections(t_context *ctx, t_command_tree *node)
{
	t_str_list	*token;
	t_dyn_ptr	argv;

	init_argv(ctx, node, node->cmd_tokens);
	token = node->cmd_tokens;
	init_dyn_ptr(&argv, 1);
	while (token)
	{
		if ((token->content[0] == '<' || token->content[0] == '>')
			&& token->next)
		{
			if (!manage_redirection(ctx, token, node->hd_fd))
			{
				free_split(argv.arr);
				return (NULL);
			}
			token = token->next;
		}
		else
			manage_wildcards(ctx, token->content, &argv);
		token = token->next;
	}
	return (argv.arr);
}

int	execute_leaf(t_context *ctx, t_command_tree *node)
{
	int		pid;
	char	**cmd_argv;

	cmd_argv = manage_argv_and_redirections(ctx, node);
	ctx->read_exit_status = 1;
	if (!cmd_argv || !cmd_argv[0] || try_builtins(ctx, cmd_argv))
	{
		free_split(cmd_argv);
		return (0);
	}
	ctx->read_exit_status = 0;
	pid = custom_fork(ctx);
	if (pid == 0)
	{
		close_redirections(ctx);
		close_hd_fds(ctx);
		if (ctx->status == MS_SUCCESS)
			execute_command(ctx, cmd_argv);
		else
			free_split(cmd_argv);
		silent_exit(ctx, ctx->exit_status);
	}
	signal(SIGINT, SIG_IGN);
	free_split(cmd_argv);
	return (pid);
}
