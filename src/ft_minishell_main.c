/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_main.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 14:59:51 by rselva-2          #+#    #+#             */
/*   Updated: 2026/06/11 19:40:49 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_builtins.h"
#include "ft_minishell_input.h"
#include "ft_minishell_execution.h"

static int	initial_config(t_context *ctx, int argc, char **argv, char **env)
{
	g_last_signal = 0;
	ctx->cmd_tree.cmd1 = NULL;
	ctx->cmd_tree.cmd2 = NULL;
	ctx->cmd_tree.cmd_tokens = NULL;
	ctx->cmd_tree.subshell_redirections = NULL;
	ctx->status = MS_SUCCESS;
	ctx->exit_status = 0;
	ctx->user_input = NULL;
	ctx->input_tokens = NULL;
	ctx->original_in = -1;
	ctx->original_out = -1;
	ctx->subshell = 0;
	if (save_env(ctx, env))
		return (ctx->status);
	if (set_shell(ctx, argv[0]))
		return (ctx->status);
	set_pwd(ctx);
	if (!find_env_node(ctx, "PS1"))
		if (add_env(ctx, "PS1=\\w\\$ "))
			return (ctx->status);
	if (increment_shlvl(ctx))
		return (ctx->status);
	check_interactive(ctx, argc, argv);
	return (MS_SUCCESS);
}

int	main(int argc, char **argv, char **env)
{
	t_context			ctx;

	set_default_signals();
	if (initial_config(&ctx, argc, argv, env) == MS_E_MALLOC)
	{
		if (ctx.shell_name)
			ft_putstr_fd(ctx.shell_name, 2);
		else
			ft_putstr_fd("minishell", 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd("Cannot allocate memory", 2);
		ft_putendl_fd("Shell aborted", 2);
		return (1);
	}
	io_while(&ctx);
	free_all(&ctx);
	return (ctx.exit_status);
}
