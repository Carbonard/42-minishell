/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_execute_command.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 22:35:42 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/25 21:01:56 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include "ft_minishell_execution.h"

int	find_cmd_path(t_context *ctx, char *path, char *cmd)
{
	char		**env_paths;
	int			i;

	env_paths = ft_split(find_env_value(ctx, "PATH"), ':');
	i = 0;
	while (env_paths && env_paths[i])
	{
		ft_strlcpy(path, env_paths[i], PATH_MAX);
		ft_strlcat(path, "/", PATH_MAX);
		ft_strlcat(path, cmd, PATH_MAX);
		if (!access(path, F_OK))
		{
			free_split(env_paths);
			ctx->exit_status = ES_SUCCESS;
			return (ft_strlen(path));
		}
		i++;
	}
	free_split(env_paths);
	ft_strlcpy(path, cmd, ARG_MAX);
	ctx->exit_status = ES_CMD_NOT_FOUND;
	return (0);
}

static void	get_path(t_context *ctx, char *argv0, t_exec_args *args)
{
	struct stat	state;

	args->path = args->exec_args;
	if (!ft_strncmp(argv0, "./", 2) || !ft_strncmp(argv0, "../", 3)
		|| !ft_strncmp(argv0, "/", 1))
	{
		if (ft_strlen(argv0) >= PATH_MAX - 1)
		{
			ctx->status = MS_LONG_PATH;
			ctx->exit_status = ES_CMD_NOT_EXEC;
		}
		else
			args->args_length = ft_strlcpy(args->path, argv0, PATH_MAX) + 1;
	}
	else
		args->args_length = find_cmd_path(ctx, args->path, argv0) + 1;
	stat(args->path, &state);
	if (!ctx->exit_status && !ctx->status
		&& (state.st_mode & S_IFMT) == S_IFDIR)
	{
		ctx->exit_status = ES_CMD_NOT_EXEC;
		ctx->status = MS_CMD_IS_DIR;
	}
	if (!ctx->exit_status && !ctx->status && access(args->path, X_OK))
		ctx->exit_status = ES_CMD_NOT_EXEC;
}

static void	save_argv(t_context *ctx, char **argv, t_exec_args *args)
{
	int	argv_i;

	if (ctx->status || ctx->exit_status)
		return ;
	argv_i = 0;
	while (*argv && argv_i < ARG_MAX - 1)
	{
		args->static_argv[argv_i] = args->exec_args + args->args_length;
		args->args_length += ft_strlcpy(args->static_argv[argv_i], *argv,
				ARG_MAX - args->args_length) + 1;
		argv_i++;
		argv++;
	}
	args->static_argv[argv_i] = NULL;
	if (args->args_length >= ARG_MAX || argv_i >= ARG_MAX - 1)
	{
		printf("last argv: %i: %s\n", argv_i - 1,
			args->static_argv[argv_i - 1]);
		ctx->status = MS_LONG_ARGS;
		ctx->exit_status = ES_CMD_NOT_EXEC;
	}
}

static size_t
	list_to_strarray(t_context *ctx, char *args, char *new_env[], size_t length)
{
	t_str_list	*env;
	int			env_i;

	if (ctx->status || ctx->exit_status)
		return (0);
	env = ctx->env;
	env_i = 0;
	while (env && env_i < ARG_MAX - 1 && length < ARG_MAX)
	{
		new_env[env_i] = args + length;
		length += ft_strlcpy(new_env[env_i], env->content, ARG_MAX - length);
		length++;
		env_i++;
		env = env->next;
	}
	new_env[env_i] = NULL;
	if (length >= ARG_MAX || env_i >= ARG_MAX - 1)
	{
		printf("last env: %i: %s\n", env_i - 1, new_env[env_i - 1]);
		ctx->status = MS_LONG_ARGS;
		ctx->exit_status = ES_CMD_NOT_EXEC;
	}
	return (length);
}

void	execute_command(t_context *ctx, char **argv)
{
	t_exec_args	args;

	ctx->exit_status = 0;
	get_path(ctx, argv[0], &args);
	save_argv(ctx, argv, &args);
	args.args_length = list_to_strarray(ctx, args.exec_args, args.env,
			args.args_length);
	// A
	free_split(argv);
	if (!ctx->status && !ctx->exit_status)
	{
		free_all(ctx);
		execve(args.path, args.static_argv, args.env);
	}
	if (ctx->exit_status == ES_CMD_NOT_FOUND)
		custom_error(args.path, "command not found");
	else
		shell_perror(ctx, args.path);
}
