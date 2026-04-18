/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_builtins_others.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 16:52:14 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/18 20:10:37 by rselva-2         ###   ########.fr       */
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

int	cd(t_context *ctx, char **argv)
{
	char	old_dir[PATH_MAX + 20];

	if (argv[1] == NULL)
		argv[1] = ft_strdup(find_env_value(ctx, "HOME"));
	else if (argv[2] != NULL)
		return (MS_TOO_MANY_ARGS);
	if (!ft_strncmp(argv[1], "-", 2))
	{
		free(argv[1]);
		argv[1] = ft_strdup(find_env_value(ctx, "OLDPWD"));
		if (!argv[1])
			return (MS_OLDPWD_NOT_SET);
		ft_putendl_fd(argv[1], 1);
	}
	ft_strlcpy(old_dir, "OLDPWD=", 8);
	getcwd(old_dir + 7, PATH_MAX);
	if (chdir(argv[1]))
		return (MS_E_PATH_NFOUND);
	export(ctx, old_dir);
	return (MS_SUCCESS);
}
