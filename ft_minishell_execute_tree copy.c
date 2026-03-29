/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_execute_tree copy.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 19:24:30 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/27 22:28:36 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

char	*find_cmd_path(t_context *ctx, char *cmd)
{
	char		**env_paths;
	char		*abs_path;
	char		*tmp;
	int			i;

	env_paths = ft_split(find_env_value(ctx, "PATH"), ':');
	i = 0;
	while (env_paths && env_paths[i])
	{
		tmp = ft_strjoin(env_paths[i], "/");
		if (!tmp)
			return (free_split(env_paths));
		abs_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!abs_path)
			return (free_split(env_paths));
		if (!access(abs_path, X_OK))
		{
			free_split(env_paths);
			return (abs_path);
		}
		free(abs_path);
		i++;
	}
	return (free_split(env_paths));
}

char	**list_to_strarray(t_str_list *env)
{
	t_str_list	*aux;
	char		**new_env;
	int			size;

	size = 0;
	aux = env;
	while (aux)
	{
		size++;
		aux = aux->next;
	}
	new_env = malloc((size + 1) * sizeof(char *));
	if (!new_env)
		return (NULL);
	size = 0;
	while (env)
	{
		new_env[size] = ft_strdup(env->content);
		if (!new_env[size])
			return (free_split(new_env));
		size++;
		env = env->next;
	}
	new_env[size] = NULL;
	return (new_env);
}

void	create_son(t_context *ctx, char **command)
{
	char	*path;
	char	**env;
	int		pid;
// Manejar mejor returns y errores
	pid = fork();
	if (pid)
	{
		add_int(&ctx->pids, pid);
		return ;
	}
	path = find_cmd_path(ctx, command[0]);
	if (!path)
		ctx->status = MS_E_PATH_NFOUND;
	else
	{
		env = list_to_strarray(ctx->env);
		if (!env)
			ctx->status = MS_E_ENV_NFOUND;
		else
			execve(path, command, env);
		// Gestionar errores
		printf("Mensaje de error\n");
		exit (-42);
	}
}

void	execute_command(t_context *ctx, t_command_tree *command)
{
	char			**split;
	t_redirection	redir;
	int				status;

	split = split_cmd(command->cmd, &redir);
	// Manage redirections
	if (!split[0])
	//habria que ver mas
		return ;
	status = try_builtins(ctx, split);
	if (status == MS_BUILT_IN_EXECUTED)
	{
		ctx->last_cmd_is_builtin = 1;
		return ;
	}
	ctx->last_cmd_is_builtin = 0;
	create_son(ctx, split);
}

int	wait_sons(t_context *ctx)
{
	size_t	i;
	int		status;

	i = 0;
	while (i < ctx->pids.length)
	{
		waitpid(ctx->pids.arr[i], &status, 0);
		i++;
	}
	ctx->pids.length = 0;
	if (ctx->last_cmd_is_builtin)
		return (ctx->status);
	return (status);
}

void	manage_pipe(t_context *ctx, t_command_tree *command)
{
	// int	pipe_fd[2];

	// pipe(pipe_fd);
	// dup2(pipe_fd[1], STDOUT_FILENO);
	// close(pipe_fd[1]);
	execute(ctx, command->cmd1);
	// dup2(pipe_fd[0], STDIN_FILENO);
	// close(pipe_fd[0]);
	execute(ctx, command->cmd2);
}

void	execute(t_context *ctx, t_command_tree *command)
{
	int	status;

	if (command->sep == NONE)
		execute_command(ctx, command);
	else if (command->sep == PIPE)
		manage_pipe(ctx, command);
	else if (command->sep == AND || command->sep == OR)
	{
		execute(ctx, command->cmd1);
		status = wait_sons(ctx);
		if ((WEXITSTATUS(status) == EXIT_SUCCESS && command->sep == AND)
			|| (WEXITSTATUS(status) != EXIT_SUCCESS && command->sep == OR))
			execute(ctx, command->cmd2);
	}
}

void	execute_input(t_context *ctx)
{
	init_dyn_int(&ctx->pids, 1);
	execute(ctx, ctx->cmd_tree);
	ctx->exit_status = wait_sons(ctx);
	free_dyn_int(&ctx->pids);
}
