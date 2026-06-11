/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_redirections.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 16:09:28 by rselva-2          #+#    #+#             */
/*   Updated: 2026/06/12 00:53:52 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_execution.h"
#include "ft_minishell_input.h"

static int
	manage_redirection_in(t_context *ctx, char *type, char *file, int hd_fd)
{
	int	fd;
	int	error;

	error = 0;
	if (!ft_strncmp(type, "<", 2))
	{
		fd = open(file, O_RDONLY);
		if (fd < 0)
		{
			shell_perror(ctx, file);
			return (1);
		}
		error = custom_dup2(ctx, fd, STDIN_FILENO);
		close(fd);
	}
	else if (!ft_strncmp(type, "<<", 3) && hd_fd != -1)
		error = custom_dup2(ctx, hd_fd, STDIN_FILENO);
	return (error);
}

static int	manage_redirection_out(t_context *ctx, char *type, char *file)
{
	int	fd;
	int	error;

	error = 0;
	if (!ft_strncmp(type, ">", 2))
	{
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0664);
		if (fd < 0)
			shell_perror(ctx, file);
		if (fd < 0)
			return (1);
		error = custom_dup2(ctx, fd, STDOUT_FILENO);
		close(fd);
	}
	else if (!ft_strncmp(type, ">>", 3))
	{
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0664);
		if (fd < 0)
			shell_perror(ctx, file);
		if (fd < 0)
			return (1);
		error = custom_dup2(ctx, fd, STDOUT_FILENO);
		close(fd);
	}
	return (error);
}

int	manage_redirection(t_context *ctx, t_str_list *token, int hd_fd)
{
	if (token->content[0] == '<')
	{
		if (manage_redirection_in(
				ctx, token->content, token->next->content, hd_fd))
		{
			ctx->status = MS_BADFILE;
			return (0);
		}
	}
	if (token->content[0] == '>')
	{
		if (manage_redirection_out(
				ctx, token->content, token->next->content))
		{
			ctx->status = MS_BADFILE;
			return (0);
		}
	}
	return (1);
}

void	restore_redirections(t_context *ctx)
{
	if (ctx->original_in == -2)
	{
		ft_putstr_fd("Error restoring redirections\nAborting shell\n", 2);
		ctx->original_in = -1;
		silent_exit(ctx, 1);
	}
	if (ctx->original_in != -1)
	{
		custom_dup2(ctx, ctx->original_in, STDIN_FILENO);
		close(ctx->original_in);
		ctx->original_in = -1;
	}
	if (ctx->original_out == -2)
	{
		ft_putstr_fd("Error restoring redirections\nAborting shell\n", 2);
		ctx->original_out = -1;
		silent_exit(ctx, 1);
	}
	if (ctx->original_out != -1)
	{
		custom_dup2(ctx, ctx->original_out, STDOUT_FILENO);
		close(ctx->original_out);
		ctx->original_out = -1;
	}
}

void	close_redirections(t_context *ctx)
{
	if (ctx->original_in != -1)
	{
		close(ctx->original_in);
		ctx->original_in = -1;
	}
	if (ctx->original_out != -1)
	{
		close(ctx->original_out);
		ctx->original_out = -1;
	}
}
