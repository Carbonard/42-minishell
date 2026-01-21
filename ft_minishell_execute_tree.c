/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_execute_tree.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 19:24:30 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/21 23:23:33 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

char	*find_cmd_path(t_context *ctx, char *cmd)
{
	char		**paths;
	char		*path;
	int			i;
// errores malloc
	paths = ft_split(find_env_value(ctx, "PATH"), ':');
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(path, cmd);
		if (!access(path, X_OK))
		{
			free_split(paths);
			return (path);
		}
		free(path);
		i++;
	}
	free_split(paths);
	return (NULL);
}

char	**list_to_strarray(t_str_list *env)
{
	t_str_list	*aux;
	char		**new_env;
	int			size;
// Control de errores
	size = 0;
	aux = env;
	while (aux)
	{
		size++;
		aux = aux->next;
	}
	new_env = malloc((size + 1) * sizeof(char *));
	size = 0;
	aux = env;
	while (aux)
	{
		new_env[size] = ft_strdup(aux->content);
		size++;
		aux = aux->next;
	}
	new_env[size] = NULL;
	return (new_env);
}

void	execute_command(t_context *ctx, char **command)
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
		return ;
	env = list_to_strarray(ctx->env);
	if (!env)
		return ;
	execve(path, command, env);
	printf("algo fue mal");
}

int	execute(t_context *ctx, t_command_tree *command)
{
	char			**split;
	t_redirection	redir;
	int				status;
// printf("\e[32m%s\nsep: %i\e[0m\n", command->cmd, command->sep);
	if (!command->sep)
	{
		split = split_cmd(command->cmd, &redir);
		// Manage redirections
		status = check_build_ins(ctx, split);
		if (status == MS_BUILT_IN_EXECUTED)
			return (0);
		execute_command(ctx, split);
	}
	else
	{
		if (command->cmd1)
			execute(ctx, command->cmd1);
		if (command->cmd2)
			execute(ctx, command->cmd2);
	}
	return (0);
}

void	execute_input(t_context *ctx)
{
	// int	i;
	int	wstatus;

	init_dyn_int(&ctx->pids, 1);
	execute(ctx, ctx->cmd_tree);
	// i = 0;
	// while (i < ctx->pids.length)
	// {
		waitpid(-1, &wstatus, 0);
	// 	i++;
	// }
	free_dyn_int(&ctx->pids);
}
