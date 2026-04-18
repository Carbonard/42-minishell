/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_main.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 14:59:51 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/18 19:21:54 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_builtins.h"
#include "ft_minishell_input.h"
#include "ft_minishell_execution.h"

static void	io_while(t_context *ctx);

void manage_multiple_input(t_context *ctx)
{
	int	pipe_fds[2];
	int	original_in;

	original_in = dup(STDIN_FILENO);
	pipe(pipe_fds);
	dup2(pipe_fds[0], STDIN_FILENO);
	ft_putstr_fd(ctx->user_input, pipe_fds[1]);
	close(pipe_fds[0]);
	close(pipe_fds[1]);
	clear_input(ctx);
	ctx->no_tty = 1;
	io_while(ctx);
	ctx->no_tty = 0;
	dup2(original_in, STDIN_FILENO);
	close(original_in);
}

static void	io_while(t_context *ctx)
{
	while (ctx->status != MS_EXIT)
	{
		ctx->status = MS_SUCCESS;
		init_dyn_ptr(&ctx->here_docs, 0);
		init_dyn_ptr(&ctx->eofs, 0);
		if (read_input(ctx))
		{
			if (ft_strchr(ctx->user_input, '\n'))
				manage_multiple_input(ctx);
			else
			{
				read_here_docs(ctx);
				add_input_history(ctx);
				if (g_last_signal)
				{
					clear_input(ctx);
					continue;
				}
				expand_heredoc(ctx);
				ctx->cmd_tree.cmd = ctx->user_input;
				create_tree(&ctx->cmd_tree);
				spread_here_docs(&ctx->cmd_tree, &ctx->here_docs, &ctx->eofs, 0);
				execute_input(ctx);
			}
		}
		else if (ctx->status == MS_EXIT)
			break ;
		clear_input(ctx);
	}
	ctx->status = MS_SUCCESS;
}

int	main(int argc, char **argv, char **env)
{
	t_context			ctx;
	struct sigaction	act, actt;

	g_last_signal = 0;
	ctx = (t_context){.cmd_tree.cmd1 = NULL, .cmd_tree.cmd2 = NULL,
		.status = MS_SUCCESS, .exit_status = 0, .user_input = NULL};
	actt = (struct sigaction){.sa_handler = generic_handler_sigint};
	act = (struct sigaction){.sa_handler = SIG_IGN};
	sigaction(SIGINT, &actt, NULL);
	act.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &act, NULL);
	save_env(&ctx, env);
	set_shell(&ctx, argv[0]);
	check_interactive(&ctx, argc, argv);
	io_while(&ctx);
	free_all(&ctx);
	return (ctx.exit_status);
}
