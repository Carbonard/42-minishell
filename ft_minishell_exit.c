/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_exit.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 00:02:17 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/29 14:00:37 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

static void	clear_tree(t_command_tree *tree)
{
	if (tree->cmd1)
		clear_tree(tree->cmd1);
	if (tree->cmd2)
		clear_tree(tree->cmd2);
	free(tree);
}

void	clear_input(t_context *ctx)
{
	if (ctx->user_input)
		free(ctx->user_input);
	ctx->user_input = NULL;
	if (ctx->cmd_tree.cmd1)
		clear_tree(ctx->cmd_tree.cmd1);
	ctx->cmd_tree.cmd1 = NULL;
	if (ctx->cmd_tree.cmd2)
		clear_tree(ctx->cmd_tree.cmd2);
	ctx->cmd_tree.cmd2 = NULL;
	free_dyn_ptr(&ctx->here_docs);
}

void	ft_exit(t_context *ctx)
{
	clear_input(ctx);
	ft_str_lstclear(&ctx->env);
	rl_clear_history();
	exit (ctx->exit_status);
}
