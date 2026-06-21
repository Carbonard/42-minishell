/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_execute_command.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 22:35:42 by rselva-2          #+#    #+#             */
/*   Updated: 2026/06/21 17:30:36 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include "ft_minishell_execution.h"

char	*find_cmd_path(t_context *ctx, char *cmd)
{
	char		**env_paths;
	char		*path;
	int			i;

	env_paths = ft_split(find_env_value(ctx, "PATH"), ':');
	i = 0;
	while (env_paths && env_paths[i])
	{
		path = triple_join(env_paths[i], "/", cmd);
		if (!path)
			malloc_exit(ctx);
		if (!access(path, F_OK))
		{
			free_split(env_paths);
			ctx->exit_status = ES_SUCCESS;
			return (path);
		}
		i++;
		free(path);
	}
	free_split(env_paths);
	path = ft_strdup(cmd);
	ctx->exit_status = ES_CMD_NOT_FOUND;
	return (path);
}

static void	get_path(t_context *ctx, char *argv0, t_exec_args *args)
{
	struct stat	state;

	if (!ft_strncmp(argv0, "./", 2) || !ft_strncmp(argv0, "../", 3)
		|| !ft_strncmp(argv0, "/", 1))
	{
		args->path = ft_strdup(argv0);
	}
	else
		args->path = find_cmd_path(ctx, argv0);
	if (!args->path)
		malloc_exit(ctx);
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

static void	list_to_strarray(t_context *ctx, t_dyn_ptr *exec_env)
{
	t_str_list	*env;

	init_dyn_ptr(exec_env, 0);
	if (ctx->status || ctx->exit_status)
		return ;
	env = ctx->env;
	while (env)
	{
		add_ptr(exec_env, env->content);
		env = env->next;
	}
}

void	execute_command(t_context *ctx, char **argv)
{
	t_exec_args	args;

	ctx->exit_status = 0;
	get_path(ctx, argv[0], &args);
	list_to_strarray(ctx, &args.env);
	if (!ctx->status && !ctx->exit_status)
	{
		free_almost_all(ctx);
		execve(args.path, argv, args.env.arr);
	}
	if (ctx->exit_status == ES_CMD_NOT_FOUND)
		custom_error(args.path, "command not found");
	else
		shell_perror(ctx, args.path);
	free_split(argv);
	free(args.path);
	free_split(args.env.arr);
}
