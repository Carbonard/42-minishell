/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_builtins_others.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 16:52:14 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/12 22:15:36 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_builtins.h"

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

int	echo(char **argv)
{
	int	arg_i;
	int	new_line;

	new_line = 1;
	arg_i = 1;
	while (argv[arg_i] && argv[arg_i][0] == '-')
	{
		if (valid_flag(argv[arg_i], 'n'))
			new_line = 0;
		else
			break ;
		arg_i++;
	}
	while (argv[arg_i])
	{
		ft_putstr_fd(argv[arg_i], 1);
		arg_i++;
		if (argv[arg_i])
			ft_putchar_fd(' ', 1);
	}
	if (new_line)
		ft_putchar_fd('\n', 1);
	return (MS_SUCCESS);
}

int	pwd(void)
{
	char	path[PATH_MAX];

	if (!getcwd(path, PATH_MAX))
		return (MS_E_PWD_NFOUND);
	ft_putendl_fd(path, 1);
	return (MS_SUCCESS);
}

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

int	cd(t_context *ctx, char **argv)
{
	int	print_path;

	print_path = 0;
	if (argv[1] == NULL)
		argv[1] = ft_strdup(find_env_value(ctx, "HOME"));
	else if (argv[2] != NULL)
		return (MS_TOO_MANY_ARGS);
	if (!argv[1])
		return (MS_HOME_NOT_SET);
	if (!ft_strncmp(argv[1], "-", 2))
	{
		free(argv[1]);
		argv[1] = ft_strdup(find_env_value(ctx, "OLDPWD"));
		if (!argv[1])
			return (MS_OLDPWD_NOT_SET);
		print_path = 1;
	}
	if (chdir(argv[1]))
		return (MS_E_PATH_NFOUND);
	if (print_path)
		ft_putendl_fd(argv[1], 1);
	set_new_vars(ctx, argv[1]);
	free (argv[1]);
	argv[1] = NULL;
	return (MS_SUCCESS);
}
