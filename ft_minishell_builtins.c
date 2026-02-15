/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_builtins.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 21:35:56 by rselva-2          #+#    #+#             */
/*   Updated: 2026/02/14 22:59:34 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

int	cd(t_context *ctx, char *new_dir)
{
	char	old_dir[MAX_PWD + 20];

	if (!ft_strncmp(new_dir, "-", 2))
	{
		new_dir = find_env_value(ctx, "OLDPWD");
		if (!new_dir)
		{
			ft_putstr_fd(ctx->shell_name, 2);
			ft_putendl_fd(": cd: OLDPWD not set", 2);
			return (MS_E_PATH_NFOUND);
		}
	}
	ft_strlcpy(old_dir, "OLDPWD=", 8);
	getcwd(old_dir + 7, MAX_PWD);
	if (chdir(new_dir))
		return (MS_E_PATH_NFOUND);
	export(ctx, old_dir);
	return (MS_SUCCESS);
}

void	builtin_exit(t_context *ctx, char **argv)
{
	int			length;
	long long	number;

	if (!argv[1])
		ft_exit(ctx, ctx->exit_status);
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
			ft_exit(ctx, 2);
		}
		length++;
	}
	number = ft_atoll(argv[1]);
	if (number > __LONG_MAX__)
	{
		ft_putstr_fd("exit\nminishell: exit: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		if (!argv[2])
			ft_exit(ctx, 2);
	}
	if (argv[2])
	{
		ctx->exit_status = 1;
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		return ;
	}
	ft_exit (ctx, (long)number);
}

int	try_builtins(t_context *ctx, char **argv)
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
		builtin_exit(ctx, argv);
	else if (!ft_strncmp(argv[0], "cd", 3))
		ctx->status = cd(ctx, argv[1]);
	else
		return (0);
	if (ctx->status != MS_SUCCESS && ft_strncmp(argv[0], "cd", 3))
		perror(argv[0]);
	// free(argv[0]);
	free(argv);
	ctx->exit_status = ctx->status;
	ctx->read_exit_status = 1;
	return (1);
}
