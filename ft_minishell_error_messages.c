/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_error_messages.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 16:59:44 by rselva-2          #+#    #+#             */
/*   Updated: 2026/03/31 21:16:40 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	minishell_perror(t_context *ctx, char *s)
{
	ft_putstr_fd(ctx->shell_name, 2);
	ft_putstr_fd(": ", 2);
	perror(s);
}

void	minishell_arg_perror(t_context *ctx, char *arg, char *s)
{
	ft_putstr_fd(ctx->shell_name, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": ", 2);
	perror(s);
}

void	minishell_error(t_context *ctx, char *s)
{
	ft_putstr_fd(ctx->shell_name, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(s, 2);
}

void	builtins_errors(t_context *ctx, char **argv)
{
	if (ctx->exit_status == MS_SUCCESS)
	{
		ctx->exit_status = 0;
		return ;
	}
	if (ctx->status == MS_TOO_MANY_ARGS)
		minishell_error(ctx, "cd: too many arguments");
	else if (ctx->status == MS_OLDPWD_NOT_SET)
		minishell_error(ctx, "cd: OLDPWD not set");
	else if (!ft_strncmp(argv[0], "cd", 3))
		minishell_arg_perror(ctx, argv[0], argv[1]);
	else
		minishell_perror(ctx, argv[0]);
	ctx->exit_status = 1;
}
