/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_main.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 14:59:51 by rselva-2          #+#    #+#             */
/*   Updated: 2026/06/04 18:19:24 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_builtins.h"
#include "ft_minishell_input.h"
#include "ft_minishell_execution.h"

static void	initial_config(t_context *ctx, int argc, char **argv, char **env)
{
	g_last_signal = 0;
	ctx->cmd_tree.cmd1 = NULL;
	ctx->cmd_tree.cmd2 = NULL;
	ctx->cmd_tree.cmd_tokens = NULL;
	ctx->status = MS_SUCCESS;
	ctx->exit_status = 0;
	ctx->user_input = NULL;
	ctx->input_tokens = NULL;
	save_env(ctx, env);
	set_shell(ctx, argv[0]);
	set_pwd(ctx);
	if (!find_env_node(ctx, "PS1"))
		export(ctx, "PS1=\\w\\$ ");
	increment_shlvl(ctx);
	check_interactive(ctx, argc, argv);
	ctx->original_in = -1;
	ctx->original_out = -1;
	ctx->subshell = 0;
}

int	main(int argc, char **argv, char **env)
{
	t_context			ctx;

	set_default_signals();
	initial_config(&ctx, argc, argv, env);
	io_while(&ctx);
	free_all(&ctx);
	return (ctx.exit_status);
}
