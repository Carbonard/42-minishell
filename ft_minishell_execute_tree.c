/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_execute_tree.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 19:24:30 by rselva-2          #+#    #+#             */
/*   Updated: 2026/03/29 23:57:55 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

int	get_status(t_context *ctx, int wstatus)
{
	if (ctx->read_exit_status)
		return (ctx->exit_status);
	ctx->read_exit_status = 1;
	if (WIFEXITED(wstatus))
		ctx->exit_status = WEXITSTATUS(wstatus);
	else if (WIFSIGNALED(wstatus))
		ctx->exit_status = WTERMSIG(wstatus) + 128;
	else
		ctx->exit_status = -42;
	//no se yo
	return (ctx->exit_status);
	// https://www.gnu.org/software/bash/manual/html_node/Exit-Status.html
}

int	execute_subshell(t_context *ctx, t_command_tree *node)
{
	int	pid;
	int	status;

	ctx->read_exit_status = 0;
	pid = fork();
	if (pid == 0)
	{
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

int	execute_logic(t_context *ctx, t_command_tree *node)
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

void	execute_pipe_child(t_context *ctx, int fileno, t_command_tree *next_node)
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

int	execute_pipe(t_context *ctx, t_command_tree *node)
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

void	execute_input(t_context *ctx)
{
	int	pid;
	int	status;

	// ctx->exit_status = 0;
	pid = execute_node(ctx, &ctx->cmd_tree);
	if (!ctx->read_exit_status)
	{
		waitpid(pid, &status, 0);
		ctx->exit_status = get_status(ctx, status);
	}
}
