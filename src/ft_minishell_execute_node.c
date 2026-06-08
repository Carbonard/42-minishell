/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_execute_node.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 16:26:36 by rselva-2          #+#    #+#             */
/*   Updated: 2026/06/08 17:58:40 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_execution.h"
#include "ft_minishell_input.h"

static int	execute_subshell(t_context *ctx, t_command_tree *node)
{
	int				pid;
	int				status;
	t_str_list		*redirections;

	ctx->read_exit_status = 0;
	pid = custom_fork(ctx);
	if (pid == 0)
	{
		ctx->subshell = 1;
		redirections = node->subshell_redirections;
		while (redirections && redirections->next)
		{
			if (!manage_redirection(ctx, redirections, node->hd_fd))
				silent_exit(ctx, ctx->exit_status);
			redirections = redirections->next->next;
		}
		pid = execute_node(ctx, node->cmd1);
		custom_waitpid(pid, &status, 0);
		ctx->exit_status = get_status(ctx, status);
		silent_exit(ctx, ctx->exit_status);
	}
	custom_waitpid(pid, &status, 0);
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
		custom_waitpid(pid, &status, 0);
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

	ctx->subshell = 1;
	if (fileno == STDIN_FILENO)
		pipe_extrem = 0;
	else
		pipe_extrem = 1;
	dup2(ctx->pipe_fds[pipe_extrem], fileno);
	close(ctx->pipe_fds[1]);
	close(ctx->pipe_fds[0]);
	pid = execute_node(ctx, next_node);
	custom_waitpid(pid, &status, 0);
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
	pid1 = custom_fork(ctx);
	if (pid1 == 0)
		execute_pipe_child(ctx, STDOUT_FILENO, node->cmd1);
	close(ctx->pipe_fds[1]);
	pid2 = custom_fork(ctx);
	if (pid2 == 0)
		execute_pipe_child(ctx, STDIN_FILENO, node->cmd2);
	close(ctx->pipe_fds[0]);
	custom_waitpid(pid1, NULL, 0);
	custom_waitpid(pid2, &status, 0);
	ctx->exit_status = get_status(ctx, status);
	return (pid2);
}

int	execute_node(t_context *ctx, t_command_tree *node)
{
	int	pid;

	if (node->subshell)
		pid = execute_subshell(ctx, node);
	else if (node->sep == NONE)
	{
		pid = execute_leaf(ctx, node);
		restore_redirections(ctx);
	}
	else if (node->sep == AND || node->sep == OR)
		pid = execute_logic(ctx, node);
	else if (node->sep == PIPE)
		pid = execute_pipe(ctx, node);
	return (pid);
}
