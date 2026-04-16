/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_execute_leaf.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 03:50:34 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/15 04:52:40 by rselva-2         ###   ########.fr       */
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
		free(file);
		file = NULL;
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
		free(file);
		file = NULL;
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (type == REDIRECTION_APP)
	{
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0664);
		free(file);
		file = NULL;
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
	free(redir->file_in.arr);
	redir->file_in.arr = NULL;
	free(redir->file_out.arr);
	redir->file_out.arr = NULL;
	free(redir->type_in.arr);
	redir->type_in.arr = NULL;
	free(redir->type_out.arr);
	redir->type_out.arr = NULL;
}

int	execute_leaf(t_context *ctx, t_command_tree *node)
{
	int				pid;
	char			**cmd_argv;
	t_redirection	redir;

	cmd_argv = get_argv_and_redir(ctx, node->cmd, &redir);
	if (cmd_argv[0] == NULL)
	{
		// ctx->status = MS_EMPTY_CMD;
		ctx->read_exit_status = 1;
		return (0);
	}
	// ctx->exit_status = 0;
	if (try_builtins(ctx, cmd_argv, &redir, node->here_doc))
		return (0);
	ctx->read_exit_status = 0;
	pid = fork();
	if (pid == 0)
	{
		if (redir.type_in.length || redir.type_out.length)
			manage_redirection(ctx, &redir, node->here_doc);
		ctx->exit_status = 0;
		execute_command(ctx, cmd_argv);
		free_split(cmd_argv);
		silent_exit(ctx, ctx->exit_status);
	}
	free_split(cmd_argv);
	if (redir.file_in.arr)
		free(redir.file_in.arr);
	if (redir.file_out.arr)
		free(redir.file_out.arr);
	if (redir.type_in.arr)
		free(redir.type_in.arr);
	if (redir.type_out.arr)
		free(redir.type_out.arr);
	return (pid);
}
