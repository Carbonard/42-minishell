/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_error_messages.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 16:59:44 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/23 15:45:53 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

static void	print_aux(char *s)
{
	ft_putstr_fd(s, 2);
	ft_putstr_fd(": ", 2);
}

void	shell_perror(t_context *ctx, char *s)
{
	ft_putstr_fd(ctx->shell_name, 2);
	ft_putstr_fd(": ", 2);
	if (s && s[0])
		print_aux(s);
	if (ctx->status == MS_TOO_MANY_ARGS)
		ft_putendl_fd("too many arguments", 2);
	else if (ctx->status == MS_OLDPWD_NOT_SET)
		ft_putendl_fd("OLDPWD not set", 2);
	else if (ctx->status == MS_HOME_NOT_SET)
		ft_putendl_fd("HOME not set", 2);
	else if (ctx->status == MS_SE_EOF)
		ft_putendl_fd("syntax error: unexpected end of file", 2);
	else if (ctx->status == MS_SE_QUOTES)
		ft_putendl_fd("unexpected EOF while looking for matching `\"'", 2);
	else if (ctx->status == MS_LONG_ARGS)
		ft_putendl_fd(strerror(E2BIG), 2);
	else if (ctx->status == MS_LONG_PATH)
		ft_putendl_fd(strerror(ENAMETOOLONG), 2);
	else if (ctx->status == MS_CMD_IS_DIR)
		ft_putendl_fd("Is a directory", 2);
	else
		perror(NULL);
	if (!ctx->exit_status)
		ctx->exit_status = 1;
}

void	custom_error(char *s, char *error_msg)
{
	ft_putstr_fd(s, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(error_msg, 2);
}

void	shell_arg_error(t_context *ctx, char **argv)
{
	ft_putstr_fd(ctx->shell_name, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(argv[0], 2);
	ft_putstr_fd(": ", 2);
	if (ctx->status == MS_NOT_VALID_ID)
		ft_putchar_fd('`', 2);
	ft_putstr_fd(argv[1], 2);
	if (ctx->status == MS_NOT_VALID_ID)
		ft_putchar_fd('\'', 2);
	ft_putstr_fd(": ", 2);
	if (ctx->status == MS_NON_NUMERIC_ARG)
		ft_putendl_fd("numeric argument required", 2);
	else if (ctx->status == MS_NOT_VALID_ID)
		ft_putendl_fd("not a valid identifier", 2);
	else
		perror(NULL);
}

void	builtins_errors(t_context *ctx, char **argv)
{
	if (ctx->status == MS_SUCCESS)
	{
		ctx->exit_status = 0;
		return ;
	}
	if ((!ft_strncmp(argv[0], "cd", 3) || !ft_strncmp(argv[0], "exit", 5)
			|| !ft_strncmp(argv[0], "export", 7))
		&& ctx->status != MS_TOO_MANY_ARGS && ctx->status != MS_OLDPWD_NOT_SET
		&& ctx->status != MS_HOME_NOT_SET)
		shell_arg_error(ctx, argv);
	else
		shell_perror(ctx, argv[0]);
	ctx->exit_status = 1;
	if (ctx->status == MS_NON_NUMERIC_ARG)
		ctx->exit_status = 2;
}
