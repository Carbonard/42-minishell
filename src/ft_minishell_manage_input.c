/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_manage_input.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 19:59:01 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/28 19:24:07 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_input.h"
#include "ft_minishell_execution.h"

static void	manage_multiple_input(t_context *ctx)
{
	int	pipe_fds[2];
	int	original_in;
	int	original_tty;

	original_in = dup(STDIN_FILENO);
	pipe(pipe_fds);
	dup2(pipe_fds[0], STDIN_FILENO);
	ft_putstr_fd(ctx->user_input, pipe_fds[1]);
	close(pipe_fds[0]);
	close(pipe_fds[1]);
	clear_input(ctx);
	original_tty = ctx->no_tty;
	ctx->no_tty = 2;
	io_while(ctx);
	ctx->no_tty = original_tty;
	dup2(original_in, STDIN_FILENO);
	close(original_in);
}

static void	manage_single_input(t_context *ctx)
{
	read_heredocs(ctx);
	add_input_history(ctx);
	if (g_last_signal)
		return ;
	expand_heredoc(ctx);
	ctx->cmd_tree.cmd_tokens = ctx->input_tokens;
	create_tree(&ctx->cmd_tree);
	spread_heredocs(&ctx->cmd_tree, &ctx->here_docs, &ctx->eofs, 0);
	execute_input(ctx);
}

static int	count_lines(char *str)
{
	int	cnt;

	cnt = 1;
	while (*str)
	{
		if (*str == '\n' && *(str + 1))
			cnt++;
		str++;
	}
	return (cnt);
}

void	io_while(t_context *ctx)
{
	while (ctx->status != MS_EXIT)
	{
		set_default_signals();
		ctx->status = MS_SUCCESS;
		init_dyn_ptr(&ctx->here_docs, 0);
		init_dyn_ptr(&ctx->eofs, 0);
		if (read_input(ctx))
		{
			if (ctx->user_input[0] != 0 && ft_strchr(ctx->user_input, '\n')
				&& ctx->input_lines < count_lines(ctx->user_input))
				manage_multiple_input(ctx);
			else
				manage_single_input(ctx);
		}
		else if (ctx->status == MS_EXIT)
			break ;
		else
			add_history(ctx->user_input);
		clear_input(ctx);
	}
	ctx->status = MS_SUCCESS;
}
