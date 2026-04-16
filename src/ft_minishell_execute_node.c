/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_execute_node.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 16:26:36 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/16 15:53:54 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_execution.h"

static int	execute_subshell(t_context *ctx, t_command_tree *node)
{
	int				pid;
	int				status;
	t_redirection	redir;

	init_dyn_int(&(redir.type_in), 1);
	init_dyn_int(&(redir.type_out), 1);
	init_dyn_ptr(&(redir.file_in), 1);
	init_dyn_ptr(&(redir.file_out), 1);
	free(split_cmd(node->redirections, &redir));
	free(node->redirections);
	ctx->read_exit_status = 0;
	pid = fork();
	if (pid == 0)
	{
		if (redir.type_in.length || redir.type_out.length)
			manage_redirection(ctx, &redir, node->here_doc);
		node->subshell = 0;
		pid = execute_node(ctx, node);
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
	exit(status);
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
