/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_builtins.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 21:35:56 by rselva-2          #+#    #+#             */
/*   Updated: 2026/03/31 16:14:12 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

int	cd(t_context *ctx, char **argv)
{
	char	old_dir[MAX_PWD + 20];

	if (argv[1] == NULL)
		argv[1] = ft_strdup(find_env_value(ctx, "HOME"));
	if (argv[2] != NULL)
	{
		printf("minishell: cd: too many arguments\n");
		ctx->exit_status = 1;
		return (MS_TOO_MANY_ARGS);
	}
	if (!ft_strncmp(argv[1], "-", 2))
	{
		free(argv[1]);
		argv[1] = ft_strdup(find_env_value(ctx, "OLDPWD"));
		if (!argv[1])
		{
			ft_putstr_fd(ctx->shell_name, 2);
			ft_putendl_fd(": cd: OLDPWD not set", 2);
			return (MS_E_PATH_NFOUND);
		}
	}
	ft_strlcpy(old_dir, "OLDPWD=", 8);
	getcwd(old_dir + 7, MAX_PWD);
	if (chdir(argv[1]))
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
		ctx->status = cd(ctx, argv);
	else
		return (0);
	pre_perror = get_preperror(argv);
	if (ctx->status != MS_SUCCESS && ctx->status != MS_TOO_MANY_ARGS)// && ft_strncmp(argv[0], "cd", 3))
		perror(pre_perror);
	else if (ctx->status == MS_SUCCESS)
		ctx->exit_status = 0;
	free(pre_perror);
	free_split(argv);
	ctx->exit_status = 0;
	if (ctx->status != MS_SUCCESS)
		ctx->exit_status = 1;
	ctx->read_exit_status = 1;
	return (1);
}
