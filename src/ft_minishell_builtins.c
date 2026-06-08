/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_builtins.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 21:35:56 by rselva-2          #+#    #+#             */
/*   Updated: 2026/06/07 21:25:18 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_builtins.h"
#include "ft_minishell_execution.h"

int	valid_flag(const char *arg, char f)
{
	int	i;

	i = 1;
	if (!arg[i])
		return (0);
	while (arg[i])
	{
		if (arg[i] != f)
			return (0);
		i++;
	}
	return (1);
}

int	pwd(void)
{
	char	path[PATH_MAX];

	if (!getcwd(path, PATH_MAX))
		return (MS_E_PWD_NFOUND);
	ft_putendl_fd(path, 1);
	return (MS_SUCCESS);
}

static void	execute_builtins(t_context *ctx, char **argv)
{
	if (!ft_strncmp(argv[0], "pwd", 4))
		ctx->status = pwd();
	else if (!ft_strncmp(argv[0], "env", 4))
		ctx->status = print_env(ctx);
	else if (!ft_strncmp(argv[0], "echo", 5))
		ctx->status = echo(argv);
	else if (!ft_strncmp(argv[0], "export", 7))
		ctx->status = builtin_export(ctx, argv);
	else if (!ft_strncmp(argv[0], "unset", 6))
		ctx->status = unset(ctx, argv);
	else if (!ft_strncmp(argv[0], "exit", 5))
		builtin_exit(ctx, argv);
	else if (!ft_strncmp(argv[0], "cd", 3))
		ctx->status = cd(ctx, argv);
}

int	try_builtins(t_context *ctx, char **argv)
{
	if (ft_strncmp(argv[0], "pwd", 4) && ft_strncmp(argv[0], "env", 4)
		&& ft_strncmp(argv[0], "echo", 5) && ft_strncmp(argv[0], "export", 7)
		&& ft_strncmp(argv[0], "unset", 6) && ft_strncmp(argv[0], "exit", 5)
		&& ft_strncmp(argv[0], "cd", 3))
		return (0);
	execute_builtins(ctx, argv);
	if (ctx->status == MS_E_MALLOC)
	{
		free_split(argv);
		malloc_exit(ctx);
	}
	builtins_errors(ctx, argv);
	ctx->read_exit_status = 1;
	return (1);
}
