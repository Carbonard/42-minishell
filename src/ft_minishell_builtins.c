/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_builtins.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 21:35:56 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/25 21:01:48 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_builtins.h"
#include "ft_minishell_execution.h"

// static int
// 	manage_builtin_redir(t_context *ctx, t_redirection *redir, char *here_doc)
// {
// 	if (redir->type_in.length)
// 		redir->original_in = dup(STDIN_FILENO);
// 	if (redir->type_out.length)
// 		redir->original_out = dup(STDOUT_FILENO);
// 	return (manage_redirection(ctx, redir, here_doc));
// }

// static void	close_redirections(t_redirection *redir)
// {
// 	if (redir->type_in.length)
// 	{
// 		dup2(redir->original_in, STDIN_FILENO);
// 		close(redir->original_in);
// 	}
// 	if (redir->type_out.length)
// 	{
// 		dup2(redir->original_out, STDOUT_FILENO);
// 		close(redir->original_out);
// 	}
// }

static void	execute_builtins(t_context *ctx, char **argv)
{
	if (!ft_strncmp(argv[0], "pwd", 4))
		ctx->status = pwd();
	else if (!ft_strncmp(argv[0], "env", 4))
		ctx->status = print_env(ctx);
	else if (!ft_strncmp(argv[0], "echo", 5))
		ctx->status = echo(argv);
	else if (!ft_strncmp(argv[0], "export", 7))
		ctx->status = builtin_export(ctx, argv);
	else if (!ft_strncmp(argv[0], "unset", 6))
		ctx->status = unset(ctx, argv);
	else if (!ft_strncmp(argv[0], "exit", 5))
		builtin_exit(ctx, argv);
	else if (!ft_strncmp(argv[0], "cd", 3))
		ctx->status = cd(ctx, argv);
	builtins_errors(ctx, argv);
	// A
	free_split(argv);
}

int	try_builtins(t_context *ctx, char **argv)
{
	if (ft_strncmp(argv[0], "pwd", 4) && ft_strncmp(argv[0], "env", 4)
		&& ft_strncmp(argv[0], "echo", 5) && ft_strncmp(argv[0], "export", 7)
		&& ft_strncmp(argv[0], "unset", 6) && ft_strncmp(argv[0], "exit", 5)
		&& ft_strncmp(argv[0], "cd", 3))
		return (0);
	execute_builtins(ctx, argv);
	ctx->read_exit_status = 1;
	return (1);
}
