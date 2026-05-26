/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_execute_node.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 16:26:36 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/26 01:20:30 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_execution.h"
#include "ft_minishell_input.h"

// int	subshell_redirection(t_context *ctx, t_command_tree *node)
// {
// 	t_str_list	*tokens;
// 	t_str_list	*token;

// 	tokens = get_tokens(node->cmd);
// 	token = tokens;
// 	while (token && token->content[0] != '<' && token->content[0] != '>')
// 	{
// 		if (token->content[0] == '(')
// 			while (token && token->content[0] != ')')
// 				token = token->next;
// 		if (token)
// 			token = token->next;
// 	}
// 	while (token && token->next)
// 	{
// 		if (!manage_redirection(ctx, token, node->here_doc))
// 		{
// 			ft_str_lstclear(&tokens, free);
// 			return (1);
// 		}
// 		token = token->next->next;
// 	}
// 	ft_str_lstclear(&tokens, free);
// 	return (0);
// }

static int	execute_subshell(t_context *ctx, t_command_tree *node)
{
	int				pid;
	int				status;
	t_str_list		*redirections;

	ctx->read_exit_status = 0;
	pid = fork();
	if (pid == 0)
	{
		redirections = node->subshell_redirections;
		while (redirections && redirections->next)
		{
			fprintf(stderr, "redirecting to '%s'\n", redirections->next->content);
			if (!manage_redirection(ctx, redirections, node->here_doc))
				silent_exit(ctx, ctx->exit_status);
			redirections = redirections->next->next;
		}
		// fprintf(stderr, "redirected\n");
		pid = execute_node(ctx, node->cmd1);
		waitpid(pid, &status, 0);
		ctx->exit_status = get_status(ctx, status);
		silent_exit(ctx, ctx->exit_status);
	}
	waitpid(pid, &status, 0);
	ctx->exit_status = get_status(ctx, status);
	return (pid);
}

static int	execute_logic(t_context *ctx, t_command_tree *node)
{
	int	pid;
	int	status;

	pid = execute_node(ctx, node->cmd1);
	if (!ctx->read_exit_status)
	{
		waitpid(pid, &status, 0);
		ctx->exit_status = get_status(ctx, status);
	}
	if ((ctx->exit_status == EXIT_SUCCESS && node->sep == AND)
		|| (ctx->exit_status != EXIT_SUCCESS && node->sep == OR))
		pid = execute_node(ctx, node->cmd2);
	return (pid);
}

static void
	execute_pipe_child(t_context *ctx, int fileno, t_command_tree *next_node)
{
	int	pipe_extrem;
	int	pid;
	int	status;

	if (fileno == STDIN_FILENO)
		pipe_extrem = 0;
	else
		pipe_extrem = 1;
	dup2(ctx->pipe_fds[pipe_extrem], fileno);
	close(ctx->pipe_fds[1]);
	close(ctx->pipe_fds[0]);
	pid = execute_node(ctx, next_node);
	waitpid(pid, &status, 0);
	status = get_status(ctx, status);
	silent_exit(ctx, status);
}

static int	execute_pipe(t_context *ctx, t_command_tree *node)
{
	int	pid1;
	int	pid2;
	int	status;

	pipe(ctx->pipe_fds);
	ctx->read_exit_status = 0;
	pid1 = fork();
	if (pid1 == 0)
		execute_pipe_child(ctx, STDOUT_FILENO, node->cmd1);
	close(ctx->pipe_fds[1]);
	pid2 = fork();
	if (pid2 == 0)
		execute_pipe_child(ctx, STDIN_FILENO, node->cmd2);
	close(ctx->pipe_fds[0]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
	ctx->exit_status = get_status(ctx, status);
	return (pid2);
}

int	execute_node(t_context *ctx, t_command_tree *node)
{
	int	pid;

	if (node->subshell)
		pid = execute_subshell(ctx, node);
	else if (node->sep == NONE)
		pid = execute_leaf(ctx, node);
	else if (node->sep == AND || node->sep == OR)
		pid = execute_logic(ctx, node);
	else if (node->sep == PIPE)
		pid = execute_pipe(ctx, node);
	return (pid);
}
