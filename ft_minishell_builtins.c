/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_builtins.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 21:35:56 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/28 13:35:40 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

// void	exit_builtin(char *cmd)
// {
// 	// printf("%s\n", strerror(errno));
// 	perror(cmd);
// 	ft_exit(errno);
// }

void	try_builtins(t_context *ctx, char **argv)
{
	if (!ft_strncmp(argv[0], "pwd", 4))
		ctx->status = pwd();
	else if (!ft_strncmp(argv[0], "env", 4))
		ctx->status = print_env(ctx);
	else if (!ft_strncmp(argv[0], "echo", 5))
		ctx->status = echo(argv);
	else if (!ft_strncmp(argv[0], "export", 7))
		ctx->status = export(ctx, argv[1]);
	else if (!ft_strncmp(argv[0], "unset", 6))
		ctx->status = unset(ctx, argv[1]);
	else if (!ft_strncmp(argv[0], "exit", 5))
		ft_exit(ctx);
	else if (ft_strncmp(argv[0], "cd", 3))
		return ;
	if (ctx->status != MS_SUCCESS)
		perror(argv[0]);
	free(argv);
	ft_exit(ctx);
}
