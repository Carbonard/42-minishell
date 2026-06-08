/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_init_config.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 16:04:35 by rselva-2          #+#    #+#             */
/*   Updated: 2026/06/05 20:08:00 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_builtins.h"
#include "ft_minishell_execution.h"

static void	read_from_file(t_context *ctx, char **argv)
{
	int	fd;

	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		shell_perror(ctx, argv[1]);
		silent_exit(ctx, ES_CMD_NOT_FOUND);
	}
	dup2(fd, STDIN_FILENO);
	close (fd);
	ctx->no_tty = 1;
}

void	check_interactive(t_context *ctx, int argc, char **argv)
{
	int	input_pipe[2];

	ctx->no_tty = 0;
	if (!isatty(0))
		ctx->no_tty = 1;
	if (argc < 2)
		return ;
	if (!valid_flag(argv[1], 'c'))
	{
		read_from_file(ctx, argv);
		return ;
	}
	if (argc < 3)
	{
		custom_error(ctx->shell_name, "-c: option requires an argument");
		silent_exit(ctx, 2);
	}
	if (!isatty(0))
		return ;
	pipe(input_pipe);
	dup2(input_pipe[0], STDIN_FILENO);
	close(input_pipe[0]);
	ft_putstr_fd(argv[2], input_pipe[1]);
	close(input_pipe[1]);
	ctx->no_tty = 1;
}

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
	return (add_env(ctx, shell_var));
}

int	set_pwd(t_context *ctx)
{
	char	pwd[PATH_MAX];

	ctx->status = MS_SUCCESS;
	ft_strlcpy(pwd, "PWD=", PATH_MAX);
	if (getcwd(pwd + 4, PATH_MAX - 4))
	{
		if (add_env(ctx, pwd))
			return (ctx->status);
	}
	else
		ctx->status = MS_E_PWD_NFOUND;
	return (ctx->status);
}

int	increment_shlvl(t_context *ctx)
{
	char	*inherited_var;
	char	new_var[17];
	int		old_shlvl;
	char	*new_shlvl;

	inherited_var = find_env_value(ctx, "SHLVL");
	if (!inherited_var)
	{
		add_env(ctx, "SHLVL=1");
		return (ctx->status);
	}
	old_shlvl = ft_atoi(inherited_var);
	new_shlvl = ft_itoa(old_shlvl + 1);
	ft_strlcpy(new_var, "SHLVL=", 17);
	if (new_shlvl)
		ft_strlcat(new_var, new_shlvl, 17);
	else
		ft_strlcat(new_var, inherited_var, 17);
	export(ctx, new_var);
	free(new_shlvl);
	return (ctx->status);
}
