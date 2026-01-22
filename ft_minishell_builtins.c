/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_builtins.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 21:35:56 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/22 16:58:14 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

int	check_build_ins(t_context *ctx, char **command)
{
	if (!ft_strncmp(command[0], "cd", 3))
		ctx->status = chdir(command[1]);
	else if (!ft_strncmp(command[0], "pwd", 4))
		ctx->status = pwd();
	else if (!ft_strncmp(command[0], "env", 4))
		ctx->status = print_env(ctx);
	else if (!ft_strncmp(command[0], "echo", 5))
		ctx->status = echo(command);
	else if (!ft_strncmp(command[0], "export", 7))
		ctx->status = export(ctx, command[1]);
	else if (!ft_strncmp(command[0], "unset", 6))
		ctx->status = unset(ctx, command[1]);
	else if (!ft_strncmp(command[0], "exit", 5))
		ft_exit(ctx);
	else
		return (MS_BUILT_IN_NFOUND);
	return (MS_BUILT_IN_EXECUTED);
}
