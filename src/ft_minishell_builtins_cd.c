/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_builtins_cd.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 18:11:10 by rselva-2          #+#    #+#             */
/*   Updated: 2026/06/07 18:11:32 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_builtins.h"

void	set_new_vars(t_context *ctx, char *new_dir)
{
	char	old_pwd_var[PATH_MAX];
	char	new_pwd_var[PATH_MAX];
	char	*old_pwd;

	old_pwd = find_env_value(ctx, "PWD");
	ft_strlcpy(old_pwd_var, "OLDPWD=", 8);
	if (old_pwd)
		ft_strlcat(old_pwd_var, old_pwd, PATH_MAX);
	export(ctx, old_pwd_var);
	ft_strlcpy(new_pwd_var, "PWD=", PATH_MAX);
	if (!getcwd(new_pwd_var + 4, PATH_MAX - 4))
	{
		write(2, "cd: error retrieving current directory: ", 40);
		perror("getcwd: cannot access parent directories");
		if (old_pwd)
		{
			ft_strlcat(new_pwd_var, old_pwd, PATH_MAX);
			ft_strlcat(new_pwd_var, "/", PATH_MAX);
			ft_strlcat(new_pwd_var, new_dir, PATH_MAX);
		}
		else
			return ;
	}
	export(ctx, new_pwd_var);
}

static int	init_cd(t_context *ctx, char **argv)
{
	char	*aux_var;

	if (!argv[1])
	{
		aux_var = find_env_value(ctx, "HOME");
		if (!aux_var)
			return (MS_HOME_NOT_SET);
		argv[1] = ft_strdup(aux_var);
		if (!argv[1])
			return (MS_E_MALLOC);
	}
	else if (!ft_strncmp(argv[1], "-", 2))
	{
		free(argv[1]);
		argv[1] = NULL;
		aux_var = find_env_value(ctx, "OLDPWD");
		if (!aux_var)
			return (MS_OLDPWD_NOT_SET);
		argv[1] = ft_strdup(aux_var);
		if (!argv[1])
			return (MS_E_MALLOC);
		return (-1);
	}
	return (MS_SUCCESS);
}

int	cd(t_context *ctx, char **argv)
{
	int	print_path;
	int	status;

	if (argv[1] && argv[2])
		return (MS_TOO_MANY_ARGS);
	status = init_cd(ctx, argv);
	print_path = 0;
	if (status == -1)
		print_path = 1;
	else if (status != MS_SUCCESS)
		return (status);
	if (chdir(argv[1]))
		return (MS_E_PATH_NFOUND);
	if (print_path)
		ft_putendl_fd(argv[1], 1);
	set_new_vars(ctx, argv[1]);
	free (argv[1]);
	argv[1] = NULL;
	return (MS_SUCCESS);
}
