/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_execute_command.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 03:50:34 by rselva-2          #+#    #+#             */
/*   Updated: 2026/03/28 23:26:20 by rselva-2         ###   ########.fr       */
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

int	need_to_expand(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			i++;
			while (str[i] != '\'')
				i++;
		}
		// check condition
		else if (str[i] == '$' && str[i + 1] && str[i + 1] != ' ')
			return (i);
		i++;
	}
	return (-1);
}

char	*expand_var(t_context *ctx, char *str, char *expanded, int var_start)
{
	int		var_len;
	char	*aux1;
	char	*aux2;

	var_len = 0;
	while (str[var_start + 1 + var_len]
		&& str[var_start + 1 + var_len] != ' '
		&& str[var_start + 1 + var_len] != '\n'
		&& str[var_start + 1 + var_len] != '"')
		var_len++;
	aux2 = ft_substr(str, var_start + 1, var_len);
	printf("var:%s.\n", aux2);
	aux1 = ft_strdup(find_env_value(ctx, aux2));
	free(aux2);
	aux2 = ft_strjoin(expanded, aux1);
	free(expanded);
	free(aux1);
	aux1 = ft_strdup(ft_strchr(str + var_start, ' '));
	expanded = ft_strjoin(aux2, aux1);
	free(aux2);
	free(aux1);
	free(str);
	return (expanded);
}

char	*expand_cmd(t_context *ctx, char *str)
{
	int		var_start;
	char	*expanded;
	char	*aux1;
	char	*aux2;

	var_start = need_to_expand(str);
	expanded = ft_substr(str, 0, var_start);
	if (str[var_start + 1] == '?')
	{
		aux2 = ft_itoa(ctx->exit_status);
		aux1 = ft_strjoin(expanded, aux2);
		free(aux2);
		free(expanded);
		expanded = ft_strjoin(aux1, str + var_start + 2);
		free(aux1);
	}
	else
		expanded = expand_var(ctx, str, expanded, var_start);
	free(str);
	return (expanded);
}

char	**get_argv_and_redir(t_context *ctx, char *cmd, t_redirection *redir)
{
	char	*command;
	char	**argv;

	command = ft_strdup(cmd);
	while (need_to_expand(command) >= 0)
		command = expand_cmd(ctx, command);
	argv = split_cmd(command, redir);
	free(command);
	return (argv);
}

// void	print_exec_error(t_context *ctx, char *cmd)
// {
// 	if (ctx->exit_status == ES_CMD_NOT_FOUND)
// 		printf("%s: command not found\n", cmd);
// 	else if (ctx->exit_status == ES_CMD_NOT_EXEC)
// 		printf()
// }

void	minishell_perror(char *s)
{
	ft_putstr_fd("minishell: ", 2);
	perror(s);
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
		ctx->exit_status = -1;
		execute_command(ctx, cmd_argv);
		if (ctx->exit_status == ES_CMD_NOT_FOUND)
			printf("%s: command not found\n", cmd_argv[0]);
		else if (cmd_argv[0])
			minishell_perror(cmd_argv[0]);
		free_split(cmd_argv);
		silent_exit(ctx, ctx->exit_status);
	}
	free_split(cmd_argv);
	return (pid);
}
