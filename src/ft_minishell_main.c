/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_main.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 14:59:51 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/29 16:19:48 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_builtins.h"
#include "ft_minishell_input.h"
#include "ft_minishell_execution.h"

static void	initial_config(t_context *ctx, int argc, char **argv, char **env)
{
	save_env(ctx, env);
	set_shell(ctx, argv[0]);
	set_pwd(ctx);
	if (!find_env_node(ctx, "PS1"))
		export(ctx, "PS1=\\w ");
	increment_shlvl(ctx);
	check_interactive(ctx, argc, argv);
	ctx->original_in = -1;
	ctx->original_out = -1;
	ctx->subshell = 0;
}

int	main(int argc, char **argv, char **env)
{
	t_context			ctx;

	g_last_signal = 0;
	set_default_signals();
	ctx = (t_context){.cmd_tree.cmd1 = NULL, .cmd_tree.cmd2 = NULL,
		.status = MS_SUCCESS, .exit_status = 0, .user_input = NULL};
	initial_config(&ctx, argc, argv, env);
	io_while(&ctx);
	free_all(&ctx);
	return (ctx.exit_status);
}
