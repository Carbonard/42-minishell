/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_execute_leaf.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 03:50:34 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/17 00:24:47 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_execution.h"

static void
	manage_redirection_in(t_context *ctx, int type, char *file, char *here_doc)
{
	int	fd;

	if (type == REDIRECTION_IN)
	{
		fd = open(file, O_RDONLY);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else if (type == HERE_DOC)
	{
		pipe(ctx->pipe_fds);
		ft_putstr_fd(here_doc, ctx->pipe_fds[1]);
		close(ctx->pipe_fds[1]);
		dup2(ctx->pipe_fds[0], STDIN_FILENO);
		close(ctx->pipe_fds[0]);
	}
}

static void	manage_redirection_out(int type, char *file)
{
	int	fd;

	if (type == REDIRECTION_OUT)
	{
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0664);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (type == REDIRECTION_APP)
	{
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0664);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void	manage_redirection(t_context *ctx, t_redirection *redir, char *here_doc)
{
	size_t	i;

	i = 0;
	while (i < redir->file_in.length)
	{
		manage_redirection_in(ctx, redir->type_in.arr[i], redir->file_in.arr[i], here_doc);
		i++;
	}
	i = 0;
	while (i < redir->file_out.length)
	{
		manage_redirection_out(redir->type_out.arr[i], redir->file_out.arr[i]);
		i++;
	}
}

int	execute_leaf(t_context *ctx, t_command_tree *node)
{
	int				pid;
	char			**cmd_argv;

	cmd_argv = get_argv_and_redir(ctx, node->cmd, &(node->redir));
	if (cmd_argv[0] == NULL)
	{
		free_split(cmd_argv);
		ctx->read_exit_status = 1;
		return (0);
	}
	if (try_builtins(ctx, cmd_argv, &(node->redir), node->here_doc))
		return (0);
	ctx->read_exit_status = 0;
	pid = fork();
	if (pid == 0)
	{
		if (node->redir.type_in.length || node->redir.type_out.length)
			manage_redirection(ctx, &(node->redir), node->here_doc);
		ctx->exit_status = 0;
		execute_command(ctx, cmd_argv);
		free_split(cmd_argv);
		silent_exit(ctx, ctx->exit_status);
	}
	free_split(cmd_argv);
	return (pid);
}
