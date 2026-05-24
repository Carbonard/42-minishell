/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_clears.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 20:33:14 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/23 15:48:49 by rselva-2         ###   ########.fr       */
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
	// free_dyn_ptr(&(tree->redir.file_in));
	// free_dyn_ptr(&(tree->redir.file_out));
	// free_dyn_int(&(tree->redir.type_in));
	// free_dyn_int(&(tree->redir.type_out));
	free(tree);
}

void	clear_input(t_context *ctx)
{
	clear_tree(ctx->cmd_tree.cmd1);
	ctx->cmd_tree.cmd1 = NULL;
	clear_tree(ctx->cmd_tree.cmd2);
	ctx->cmd_tree.cmd2 = NULL;
	free(ctx->user_input);
	ctx->user_input = NULL;
	// free_dyn_ptr(&(ctx->cmd_tree.redir.file_in));
	// free_dyn_ptr(&(ctx->cmd_tree.redir.file_out));
	// free_dyn_int(&(ctx->cmd_tree.redir.type_in));
	// free_dyn_int(&(ctx->cmd_tree.redir.type_out));
	free_dyn_ptr(&ctx->here_docs);
	free_dyn_ptr(&ctx->eofs);
	ft_str_lstclear(&ctx->input_tokens, free);
	restore_redirections(ctx);
}

void	free_all(t_context *ctx)
{
	clear_input(ctx);
	ft_str_lstclear(&ctx->env, free);
	rl_clear_history();
}
