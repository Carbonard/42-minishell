/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_exit.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 00:02:17 by rselva-2          #+#    #+#             */
/*   Updated: 2026/03/29 22:27:59 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	silent_exit(t_context *ctx, long status)
{
	free_all(ctx);
	exit (status % 256);
}

void	ft_exit(t_context *ctx, long status)
{
	printf("exit\n");
	silent_exit(ctx, status);
}

static void	builtin_exit_2(t_context *ctx, char **argv, long number)
{
	if (number > __LONG_MAX__)
	{
		ft_putstr_fd("exit\nminishell: exit: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		if (!argv[2])
		{
			free_split(argv);
			ft_exit(ctx, 2);
		}
	}
	if (argv[2])
	{
		ctx->exit_status = 1;
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		return ;
	}
	free_split(argv);
	ft_exit(ctx, (long)number);
}

void	builtin_exit(t_context *ctx, char **argv)
{
	int			length;

	if (!argv[1])
	{
		free_split(argv);
		ft_exit(ctx, ctx->exit_status);
	}
	length = 0;
	if (argv[1][0] == '-')
		length++;
	while (argv[1][length])
	{
		if (!ft_isdigit(argv[1][length]) || length > 19)
		{
			ft_putstr_fd("exit\nminishell: exit: ", 2);
			ft_putstr_fd(argv[1], 2);
			ft_putendl_fd(": numeric argument required", 2);
			free_split(argv);
			ft_exit(ctx, 2);
		}
		length++;
	}
	builtin_exit_2(ctx, argv, ft_atoll(argv[1]));
}
