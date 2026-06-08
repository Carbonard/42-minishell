/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_execute_tree.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 19:24:30 by rselva-2          #+#    #+#             */
/*   Updated: 2026/06/08 18:17:17 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_execution.h"

int	get_status(t_context *ctx, int wstatus)
{
	if (ctx->read_exit_status)
		return (ctx->exit_status);
	ctx->read_exit_status = 1;
	if (ctx->status == MS_E_FORK_FAIL)
		ctx->exit_status = 1;
	else if (WIFEXITED(wstatus))
		ctx->exit_status = WEXITSTATUS(wstatus);
	else if (WIFSIGNALED(wstatus))
		ctx->exit_status = WTERMSIG(wstatus) + 128;
	else
		ctx->exit_status = -1;
	return (ctx->exit_status);
}

void	execute_input(t_context *ctx)
{
	int	pid;
	int	status;

	pid = execute_node(ctx, &ctx->cmd_tree);
	if (!ctx->read_exit_status)
	{
		custom_waitpid(pid, &status, 0);
		ctx->exit_status = get_status(ctx, status);
	}
}
