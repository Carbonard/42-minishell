/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_init_config.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 16:04:35 by rselva-2          #+#    #+#             */
/*   Updated: 2026/06/12 01:07:52 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_builtins.h"
#include "ft_minishell_execution.h"

int	set_shell(t_context *ctx, char *shell_name)
{
	char	shell_var[PATH_MAX + 5];
	char	current_dir[PATH_MAX];
	int		last_slash;

	ft_strlcpy(shell_var, "SHELL=", PATH_MAX);
	if (shell_name[0] != '.' && shell_name[0] != '/' && shell_name[0] != '~')
	{
		ctx->shell_name = shell_name;
		find_cmd_path(ctx, shell_var + 6, shell_name);
	}
	else
	{
		last_slash = ft_strlen(shell_name);
		while (shell_name[last_slash] != '/')
			last_slash--;
		ctx->shell_name = shell_name + last_slash + 1;
		shell_name[last_slash] = 0;
		getcwd(current_dir, PATH_MAX);
		chdir(shell_name);
		getcwd(shell_var + 6, PATH_MAX);
		chdir(current_dir);
		ft_strlcat(shell_var, "/", PATH_MAX);
		ft_strlcat(shell_var, ctx->shell_name, PATH_MAX);
	}
	return (export(ctx, shell_var));
}

int	set_pwd(t_context *ctx)
{
	char	pwd[PATH_MAX];

	ctx->status = MS_SUCCESS;
	ft_strlcpy(pwd, "PWD=", PATH_MAX);
	if (getcwd(pwd + 4, PATH_MAX - 4))
	{
		if (export(ctx, pwd))
			return (ctx->status);
	}
	else
		ctx->status = MS_E_PWD_NFOUND;
	return (ctx->status);
}

static char	*get_new_shlvl(t_context *ctx, int old_shlvl)
{
	int	new_shlvl;

	if (old_shlvl == 999)
	{
		custom_error(ctx->shell_name,
			"warning: shell level (1000) too high, resetting to 1");
		new_shlvl = 1;
	}
	else
		new_shlvl = old_shlvl + 1;
	return (ft_itoa(new_shlvl));
}

int	increment_shlvl(t_context *ctx)
{
	char	*inherited_var;
	char	new_var[17];
	char	*new_shlvl;

	inherited_var = find_env_value(ctx, "SHLVL");
	if (!inherited_var)
	{
		export(ctx, "SHLVL=1");
		return (ctx->status);
	}
	new_shlvl = get_new_shlvl(ctx, ft_atoi(inherited_var));
	if (!new_shlvl)
	{
		ctx->status = MS_E_MALLOC;
		return (ctx->status);
	}
	ft_strlcpy(new_var, "SHLVL=", 17);
	if (new_shlvl)
		ft_strlcat(new_var, new_shlvl, 17);
	else
		ft_strlcat(new_var, inherited_var, 17);
	export(ctx, new_var);
	free(new_shlvl);
	return (ctx->status);
}
