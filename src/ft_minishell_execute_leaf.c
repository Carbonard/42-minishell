/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_execute_leaf.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 03:50:34 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/23 15:45:36 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_execution.h"
#include "ft_minishell_input.h"

void	mock_free(void *ptr)
{
	(void)ptr;
}

static t_str_list	*init_argv(t_context *ctx, t_dyn_ptr *argv,
						t_command_tree *node, t_str_list **tokens)
{
	char		*expanded_cmd;

	ctx->original_in = dup(STDIN_FILENO);
	ctx->original_out = dup(STDOUT_FILENO);
	expanded_cmd = expand_input(ctx, ft_strdup(node->cmd));
	init_dyn_ptr(argv, 1);
	*tokens = get_tokens(expanded_cmd);
	free(expanded_cmd);
	return (*tokens);
}

static void	refine_token(t_dyn_ptr *argv, t_str_list *token)
{
	t_str_list	*new_tokens;
	t_str_list	*token_i;

	new_tokens = get_tokens(token->content);
	token_i = new_tokens;
	while (token_i)
	{
		if (!expand_wildcards(token_i->content, argv))
			add_ptr(argv, remove_quotes (token_i->content));
		else
			free(token_i->content);
		token_i = token_i->next;
	}
	ft_str_lstclear(&new_tokens, mock_free);
}

char	**manage_argv_and_redirections(t_context *ctx, t_command_tree *node)
{
	t_str_list	*tokens;
	t_str_list	*token;
	t_dyn_ptr	argv;

	token = init_argv(ctx, &argv, node, &tokens);
	while (token)
	{
		if ((token->content[0] == '<' || token->content[0] == '>')
			&& token->next)
		{
			if (!manage_redirection(ctx, token, node->here_doc))
			{
				ft_str_lstclear(&tokens, free);
				free_split(argv.arr);
				return (NULL);
			}
			token = token->next;
		}
		else
			refine_token(&argv, token);
		token = token->next;
	}
	ft_str_lstclear(&tokens, free);
	return (argv.arr);
}

int	execute_leaf(t_context *ctx, t_command_tree *node)
{
	int		pid;
	char	**cmd_argv;

	cmd_argv = manage_argv_and_redirections(ctx, node);
	ctx->read_exit_status = 1;
	if (!cmd_argv || try_builtins(ctx, cmd_argv))
	{
		restore_redirections(ctx);
		return (0);
	}
	ctx->read_exit_status = 0;
	pid = fork();
	if (pid == 0)
	{
		close_redirections(ctx);
		if (ctx->status == MS_SUCCESS)
			execute_command(ctx, cmd_argv);
		else
			free_split(cmd_argv);
		silent_exit(ctx, ctx->exit_status);
	}
	restore_redirections(ctx);
	signal(SIGINT, SIG_IGN);
	free_split(cmd_argv);
	return (pid);
}
