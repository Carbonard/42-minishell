/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_execute_command.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 03:50:34 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/29 12:45:16 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	execute_command(t_context *ctx, char **argv)
{
	char			*path;
	char			**env;

	if (!argv[0])
	//habria que ver mas
		return ;
	try_builtins(ctx, argv);
	if (!ft_strncmp(argv[0], "./", 2) || !ft_strncmp(argv[0], "../", 3))
		path = ft_strdup(argv[0]);
	else
		path = find_cmd_path(ctx, argv[0]);
	if (!path)
	{
		ctx->status = MS_E_PATH_NFOUND;
		return ;
	}
	env = list_to_strarray(ctx->env);
	if (!env)
		ctx->status = MS_E_ENV_NFOUND;
	else
		execve(path, argv, env);
	free(path);
	free_split(env);
}

void	manage_redirection(t_context *ctx, t_redirection *redir, char *here_doc)
{
	int	fd;

	if (redir->type_in == REDIRECTION_IN)
	{
		fd = open(redir->file_in, O_RDONLY);
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
	if (redir->type_out == REDIRECTION_OUT)
	{
		fd = open(redir->file_out, O_WRONLY | O_CREAT | O_TRUNC, 0664);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (redir->type_out == REDIRECTION_APP)
	{
		fd = open(redir->file_out, O_WRONLY | O_CREAT | O_APPEND, 0664);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

int	execute_leaf(t_context *ctx, t_command_tree *node)
{
	int				pid;
	char			**split;
	t_redirection	redir;

	split = split_cmd(node->cmd, &redir);
	if (!ft_strncmp(split[0], "exit", 5))
	{
		free(split);
		ft_exit(ctx);
	}
	ctx->status = 0;
	if (!ft_strncmp(split[0], "cd", 3))
	{
		ctx->status = chdir (split[1]);
	}
	ctx->read_exit_status = 0;
	pid = fork();
	if (pid == 0)
	{
		if (redir.type_in || redir.type_out)
			manage_redirection(ctx, &redir, node->here_doc);
		execute_command(ctx, split);
		perror(split[0]);
		free(split);
		if (ctx->status == MS_E_PATH_NFOUND)
		{
			if (errno == EACCES)
				exit (126);
			else if (errno == ENOENT)
				exit (127);
		}
		exit (-42);
	}
	free(split);
	return (pid);
}
