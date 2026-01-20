/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_exit.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 00:02:17 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/20 00:13:02 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

static void	clear_tree(t_command *tree)
{
	if (tree->cmd1)
		clear_tree(tree->cmd1);
	if (tree->cmd2)
		clear_tree(tree->cmd2);
	free(tree);
}

void	ft_exit(t_context *ctx)
{
	free(ctx->user_input);
	clear_tree(ctx->cmd_tree);
	ft_str_lstclear(&ctx->env);
	exit (errno);
}