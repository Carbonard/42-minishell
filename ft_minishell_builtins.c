/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_builtins.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 21:35:56 by rselva-2          #+#    #+#             */
/*   Updated: 2026/03/29 20:49:07 by rselva-2         ###   ########.fr       */
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

char	*get_preperror(char **argv)
{
	char	*pre_perror;
	char	*aux;

	pre_perror = ft_strdup("minishell: ");
	aux = ft_strjoin(pre_perror, argv[0]);
	free(pre_perror);
	pre_perror = aux;
	if (!ft_strncmp(argv[0], "cd", 3))
	{
		aux = ft_strjoin(pre_perror, ": ");
		free(pre_perror);
		pre_perror = aux;
		aux = ft_strjoin(pre_perror, argv[1]);
		free(pre_perror);
		pre_perror = aux;
	}
	return (pre_perror);
}

int	try_builtins(t_context *ctx, char **argv)
{
	char	*pre_perror;

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
	pre_perror = get_preperror(argv);
	if (ctx->status != MS_SUCCESS)
		perror(pre_perror);
	else
		ctx->exit_status = 0;
	free(pre_perror);
	free_split(argv);
	ctx->exit_status = 0;
	if (ctx->status != MS_SUCCESS)
		ctx->exit_status = 1;
	ctx->read_exit_status = 1;
	return (1);
}
