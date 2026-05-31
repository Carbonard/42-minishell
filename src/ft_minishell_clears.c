/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_clears.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 20:33:14 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/30 22:59:33 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_execution.h"

static void	clear_tree(t_command_tree *tree)
{
	if (!tree)
		return ;
	if (tree->cmd1)
		clear_tree(tree->cmd1);
	if (tree->cmd2)
		clear_tree(tree->cmd2);
	ft_str_lstclear(&(tree->cmd_tokens), free);
	free(tree);
}

void	close_hd_fds(t_context *ctx)
{
	size_t	i;

	i = 0;
	while (i < ctx->heredocs_fds.length)
	{
		close(ctx->heredocs_fds.arr[i]);
		i++;
	}
}

void	clear_input(t_context *ctx)
{
	clear_tree(ctx->cmd_tree.cmd1);
	ctx->cmd_tree.cmd1 = NULL;
	clear_tree(ctx->cmd_tree.cmd2);
	ctx->cmd_tree.cmd2 = NULL;
	free(ctx->user_input);
	ctx->user_input = NULL;
	if (ctx->heredocs_fds.size)
	{
		close_hd_fds(ctx);
		free_dyn_int(&ctx->heredocs_fds);
	}
	free_dyn_ptr(&ctx->eofs);
	ft_str_lstclear(&(ctx->cmd_tree.cmd_tokens), free);
	restore_redirections(ctx);
}

void	free_all(t_context *ctx)
{
	clear_input(ctx);
	ft_str_lstclear(&ctx->env, free);
	rl_clear_history();
}
