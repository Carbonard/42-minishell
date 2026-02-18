/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_execute_command.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 03:50:34 by rselva-2          #+#    #+#             */
/*   Updated: 2026/02/15 21:01:29 by rselva-2         ###   ########.fr       */
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

void	execute_command(t_context *ctx, char **argv)
{
	char			*path;
	char			**env;

	if (!argv[0])
	//habria que ver mas
		return ;
	// try_builtins(ctx, argv);
	if (!ft_strncmp(argv[0], "./", 2) || !ft_strncmp(argv[0], "../", 3)
		|| !ft_strncmp(argv[0], "/", 1))
		path = ft_strdup(argv[0]);
	else
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
		else if (str[i] == '$' && str[i + 1] != ' ')
			return (i);
		i++;
	}
	return (-1);
}

char	*expand_var(t_context *ctx, char *str)
{
	int		var_start;
	int		var_len;
	char	*expanded;
	char	*aux1;
	char	*aux2;

	var_start = need_to_expand(str);
	expanded = ft_substr(str, 0, var_start);
	var_len = 0;
	while (str[var_start + 1 + var_len]
		&& str[var_start + 1 + var_len] != ' ' && str[var_start + 1 + var_len] != '"')
		var_len++;
	aux2 = ft_substr(str, var_start + 1, var_len);
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

void	expand_argv(t_context *ctx, char **argv)
{
	int		i;

	i = 0;
	while (argv[i])
	{
		if (argv[i][0] == '$' && (argv[i][1] == '\'' || argv[i][1] == '"'))
			argv[i]++;
		argv[i] = ft_strdup(argv[i]);
		while (need_to_expand(argv[i]) >= 0)
			argv[i] = expand_var(ctx, argv[i]);
		i++;
	}
}

int	execute_leaf(t_context *ctx, t_command_tree *node)
{
	int				pid;
	char			**split;
	t_redirection	redir;
	char			*command;

	command = ft_strdup(node->cmd);
	while (need_to_expand(command) >= 0)
		command = expand_var(ctx, command);
	split = split_cmd(command, &redir);
	// free(command);
	if (try_builtins(ctx, split))
	{
		free(command);
		return (0);
	}
	ctx->read_exit_status = 0;
	pid = fork();
	if (pid == 0)
	{
		if (redir.type_in || redir.type_out)
			manage_redirection(ctx, &redir, node->here_doc);
		execute_command(ctx, split);
		if (split[0])
		perror(split[0]);
		free(command);
		free(split);
		if (ctx->status == MS_E_PATH_NFOUND)
		{
			if (errno == EACCES)
				ctx->exit_status = 126;
			else if (errno == ENOENT)
				ctx->exit_status = 127;
		}
		ctx->exit_status = -42;
		ft_exit(ctx, ctx->exit_status);
	}
	free(command);
	free(split);
	return (pid);
}
