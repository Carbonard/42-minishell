/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_builtins.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 21:35:56 by rselva-2          #+#    #+#             */
/*   Updated: 2026/02/11 13:42:49 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

// void	exit_builtin(char *cmd)
// {
// 	// printf("%s\n", strerror(errno));
// 	perror(cmd);
// 	ft_exit(errno);
// }
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
		ft_exit(ctx);
	else if (!ft_strncmp(argv[0], "cd", 3))
		ctx->status = cd(ctx, argv[1]);
	else
		return (0);
	if (ctx->status != MS_SUCCESS && ft_strncmp(argv[0], "cd", 3))
		perror(argv[0]);
	free(argv[0]);
	free(argv);
	ctx->exit_status = ctx->status;
	ctx->read_exit_status = 1;
	// ft_exit(ctx);
	// exit status
	return (1);
}
