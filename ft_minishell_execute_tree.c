/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_execute_tree.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 19:24:30 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/28 21:32:40 by rselva-2         ###   ########.fr       */
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

int	get_status(t_context *ctx, int wstatus)
{
	if (ctx->read_exit_status)
		return (ctx->exit_status);
	ctx->read_exit_status = 1;
	if(WIFEXITED(wstatus))
		ctx->exit_status = WEXITSTATUS(wstatus);
	else if (WIFSIGNALED(wstatus))
		ctx->exit_status = WTERMSIG(wstatus) + 128;
	else
		ctx->exit_status = -42;
	//no se yo
	return (ctx->exit_status);
	// https://www.gnu.org/software/bash/manual/html_node/Exit-Status.html
}

int	execute_subshell(t_context *ctx, t_command_tree *node)
{
	int	pid;
	int	status;

	ctx->read_exit_status = 0;
	printf("subshell\n");
	pid = fork();
	if (pid == 0)
	{
		node->subshell = 0;
		pid = execute_node(ctx, node);
		waitpid(pid, &status, 0);
		ctx->exit_status = get_status(ctx, status);
		ft_exit(ctx);
	}
	waitpid(pid, &status, 0);
	ctx->exit_status = get_status(ctx, status);
	return (pid);
}

void	execute_command(t_context *ctx, char **argv)
{
	char			*path;
	char			**env;

	// Manage redirections
	if (!argv[0])
	//habria que ver mas
		return ;
	try_builtins(ctx, argv);
	path = find_cmd_path(ctx, argv[0]);
	if (!path)
	{
		ctx->status = MS_E_PATH_NFOUND;
		return ;
	}
	env = list_to_strarray(ctx->env);
	if (!env)
		ctx->status = MS_E_ENV_NFOUND;
	else
		execve(path, argv, env);
	free(path);
	free_split(env);
}

int	execute_leaf(t_context *ctx, t_command_tree *node)
{
	int				pid;
	char			**split;
	t_redirection	redir;

	split = split_cmd(node->cmd, &redir);
	if (!ft_strncmp(split[0], "exit", 5))
	{
		free(split);
		ft_exit(ctx);
	}
	ctx->status = 0;
	if (!ft_strncmp(split[0], "cd", 3))
	{
		ctx->status = chdir (split[1]);
	}
	ctx->read_exit_status = 0;
	pid = fork();
	if (pid == 0)
	{
		execute_command(ctx, split);
		free(split);
		if (ctx->status == MS_E_PATH_NFOUND)
		{
			if (errno == EACCES)
				exit (126);
			else if (errno == ENOENT)
				exit (127);
		}
		exit (-42);
	}
	free(split);
	return (pid);
}

int	execute_logic(t_context *ctx, t_command_tree *node)
{
	int	pid;
	int	status;

	pid = execute_node(ctx, node->cmd1);
	if (!ctx->read_exit_status)
	{
		waitpid(pid, &status, 0);
		ctx->exit_status = get_status(ctx, status);
	}
	if ((ctx->exit_status == EXIT_SUCCESS && node->sep == AND)
		|| (ctx->exit_status != EXIT_SUCCESS && node->sep == OR))
		pid = execute_node(ctx, node->cmd2);
	return (pid);
}

int	execute_pipe(t_context *ctx, t_command_tree *node)
{
	int	pid1;
	int	pid2;
	int	status;

	pipe(ctx->pipe_fds);
	ctx->read_exit_status = 0;
	pid1 = fork();
	if (pid1 == 0)
	{
		dup2(ctx->pipe_fds[1], STDOUT_FILENO);
		close(ctx->pipe_fds[1]);
		close(ctx->pipe_fds[0]);
		pid1 = execute_node(ctx, node->cmd1);
		waitpid(pid1, &status, 0);
		exit (get_status(ctx, status));
	}
	close(ctx->pipe_fds[1]);
	pid2 = fork();
	if (pid2 == 0)
	{
		dup2(ctx->pipe_fds[0], STDIN_FILENO);
		close(ctx->pipe_fds[1]);
		close(ctx->pipe_fds[0]);
		pid2 = execute_node(ctx, node->cmd2);
		waitpid(pid2, &status, 0);
		exit (get_status(ctx, status));
	}
	close(ctx->pipe_fds[0]);
	// waitpid(pid1, NULL, 0);
	// waitpid(pid2, &status, 0);
	// ctx->exit_status = get_status(ctx, status);
	return (pid2);
}

int	execute_node(t_context *ctx, t_command_tree *node)
{
	int	pid;

	if (node->subshell)
		pid = execute_subshell(ctx, node);
	else if (node->sep == NONE)
		pid = execute_leaf(ctx, node);
	else if (node->sep == AND || node->sep == OR)
		pid = execute_logic(ctx, node);
	else if (node->sep == PIPE)
		pid = execute_pipe(ctx, node);
	return (pid);
}

void	execute_input(t_context *ctx)
{
	int	pid;
	int	status;

	ctx->exit_status = 0;
	pid = execute_node(ctx, &ctx->cmd_tree);
	if (!ctx->read_exit_status)
	{
		waitpid(pid, &status, 0);
		ctx->exit_status = get_status(ctx, status);
	}
}
