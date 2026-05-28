/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_exit.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 00:02:17 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/28 19:23:27 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_builtins.h"

void	silent_exit(t_context *ctx, long status)
{
	free_all(ctx);
	exit(status % 256);
}
// printf("Exit with status: %d\n", (int)(status % 256));

void	ft_exit(t_context *ctx, long status)
{
	printf("exit\n");
	silent_exit(ctx, status);
}

void	builtin_exit(t_context *ctx, char **argv)
{
	int	length;

	if (ctx->subshell == 0)
		ft_putendl_fd("exit", 1);
	if (argv[1])
	{
		length = 0;
		if (argv[1][0] == '-')
			length++;
		while (argv[1][length])
		{
			if (!ft_isdigit(argv[1][length]) || length > 19)
				ctx->status = MS_NON_NUMERIC_ARG;
			length++;
		}
		if (argv[2] && ctx->status != MS_NON_NUMERIC_ARG)
			ctx->status = MS_TOO_MANY_ARGS;
		if (ctx->status == MS_TOO_MANY_ARGS)
			return ;
		ctx->exit_status = ft_atoll(argv[1]);
		if (ctx->status == MS_NON_NUMERIC_ARG)
			builtins_errors(ctx, argv);
	}
	free_split(argv);
	silent_exit(ctx, ctx->exit_status);
}
