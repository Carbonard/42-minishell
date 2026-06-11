/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_init_config_interactive.c             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 16:04:35 by rselva-2          #+#    #+#             */
/*   Updated: 2026/06/12 01:05:21 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_builtins.h"
#include "ft_minishell_execution.h"

static void	read_from_file(t_context *ctx, char **argv)
{
	int	fd;

	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		shell_perror(ctx, argv[1]);
		ctx->exit_status = ES_CMD_NOT_FOUND;
		ctx->status = MS_BADFILE;
		return ;
	}
	if (custom_dup2(ctx, fd, STDIN_FILENO))
	{
		close(fd);
		ctx->exit_status = ES_CMD_NOT_FOUND;
		ctx->status = MS_BADFILE;
		return ;
	}
	close (fd);
	ctx->no_tty = 1;
}

static void	read_from_argv(t_context *ctx, char **argv)
{
	int	input_pipe[2];

	if (custom_pipe(ctx, input_pipe))
	{
		ctx->status = MS_E_PIPE;
		ctx->exit_status = 1;
		return ;
	}
	if (custom_dup2(ctx, input_pipe[0], STDIN_FILENO))
		close (STDIN_FILENO);
	close(input_pipe[0]);
	ft_putstr_fd(argv[2], input_pipe[1]);
	close(input_pipe[1]);
	ctx->no_tty = 1;
}

void	check_interactive(t_context *ctx, int argc, char **argv)
{
	ctx->no_tty = 0;
	if (!isatty(0))
		ctx->no_tty = 1;
	if (argc < 2)
		return ;
	if (!valid_flag(argv[1], 'c'))
	{
		read_from_file(ctx, argv);
		return ;
	}
	if (argc < 3)
	{
		custom_error(ctx->shell_name, "-c: option requires an argument");
		silent_exit(ctx, 2);
	}
	if (!isatty(0))
		return ;
	read_from_argv(ctx, argv);
}
