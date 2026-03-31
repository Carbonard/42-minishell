/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_execute_command.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 03:50:34 by rselva-2          #+#    #+#             */
/*   Updated: 2026/03/31 17:00:07 by rselva-2         ###   ########.fr       */
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
		if (!access(abs_path, F_OK))
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

void	manage_redirection(t_context *ctx, t_redirection *redir, char *here_doc)
{
	int	fd;

	if (redir->type_in == REDIRECTION_IN)
	{
		fd = open(redir->file_in, O_RDONLY);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else if (redir->type_in == HERE_DOC)
	{
		pipe(ctx->pipe_fds);
		ft_putstr_fd(here_doc, ctx->pipe_fds[1]);
		close(ctx->pipe_fds[1]);
		dup2(ctx->pipe_fds[0], STDIN_FILENO);
		close(ctx->pipe_fds[0]);
	}
	if (redir->type_out == REDIRECTION_OUT)
	{
		fd = open(redir->file_out, O_WRONLY | O_CREAT | O_TRUNC, 0664);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (redir->type_out == REDIRECTION_APP)
	{
		fd = open(redir->file_out, O_WRONLY | O_CREAT | O_APPEND, 0664);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void	execute_command(t_context *ctx, char **argv)
{
	char			*path;
	char			**env;

	if (!argv || !argv[0])
		return ;
	if (!ft_strncmp(argv[0], "./", 2) || !ft_strncmp(argv[0], "../", 3)
		|| !ft_strncmp(argv[0], "/", 1))
		path = ft_strdup(argv[0]);
	else
		path = find_cmd_path(ctx, argv[0]);
	if (path && access(path, X_OK))
		ctx->exit_status = ES_CMD_NOT_EXEC;
	env = NULL;
	if (!path)
		ctx->exit_status = ES_CMD_NOT_FOUND;
	else
		env = list_to_strarray(ctx->env);
	if (path && !env)
		ctx->status = MS_E_ENV_NFOUND;
	else if (path && env)
		execve(path, argv, env);
	free(path);
	free_split(env);
}

int	execute_leaf(t_context *ctx, t_command_tree *node)
{
	int				pid;
	char			**cmd_argv;
	t_redirection	redir;

	cmd_argv = get_argv_and_redir(ctx, node->cmd, &redir);
	if (try_builtins(ctx, cmd_argv))
		return (0);
	ctx->read_exit_status = 0;
	pid = fork();
	if (pid == 0)
	{
		if (redir.type_in || redir.type_out)
			manage_redirection(ctx, &redir, node->here_doc);
		ctx->exit_status = 1;
		execute_command(ctx, cmd_argv);
		if (ctx->exit_status == ES_CMD_NOT_FOUND)
			printf("%s: command not found\n", cmd_argv[0]);
		else if (cmd_argv[0])
			minishell_perror(ctx, cmd_argv[0]);
		free_split(cmd_argv);
		silent_exit(ctx, ctx->exit_status);
	}
	free_split(cmd_argv);
	return (pid);
}
