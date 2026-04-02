/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_execute_leaf.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 03:50:34 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/02 22:54:01 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

static void
	manage_redirection_in(t_context *ctx, t_redirection *redir, char *here_doc)
{
	int	fd;

	if (redir->type_in == REDIRECTION_IN)
	{
		fd = open(redir->file_in, O_RDONLY);
		free(redir->file_in);
		redir->file_in = NULL;
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else if (redir->type_in == HERE_DOC)
	{
		pipe(ctx->pipe_fds);
		ft_putstr_fd(here_doc, ctx->pipe_fds[1]);
		close(ctx->pipe_fds[1]);
		dup2(ctx->pipe_fds[0], STDIN_FILENO);
		close(ctx->pipe_fds[0]);
	}
}

static void	manage_redirection_out(t_redirection *redir)
{
	int	fd;

	if (redir->type_out == REDIRECTION_OUT)
	{
		fd = open(redir->file_out, O_WRONLY | O_CREAT | O_TRUNC, 0664);
		free(redir->file_out);
		redir->file_out = NULL;
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (redir->type_out == REDIRECTION_APP)
	{
		fd = open(redir->file_out, O_WRONLY | O_CREAT | O_APPEND, 0664);
		free(redir->file_out);
		redir->file_out = NULL;
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void	manage_redirection(t_context *ctx, t_redirection *redir, char *here_doc)
{
	manage_redirection_in(ctx, redir, here_doc);
	manage_redirection_out(redir);
}

int	execute_leaf(t_context *ctx, t_command_tree *node)
{
	int				pid;
	char			**cmd_argv;
	t_redirection	redir;

	cmd_argv = get_argv_and_redir(ctx, node->cmd, &redir);
	// ctx->exit_status = 0;
	if (try_builtins(ctx, cmd_argv, &redir, node->here_doc))
		return (0);
	ctx->read_exit_status = 0;
	pid = fork();
	if (pid == 0)
	{
		if (redir.type_in || redir.type_out)
			manage_redirection(ctx, &redir, node->here_doc);
		ctx->exit_status = 0;
		execute_command(ctx, cmd_argv);
		free_split(cmd_argv);
		silent_exit(ctx, ctx->exit_status);
	}
	free_split(cmd_argv);
	return (pid);
}
