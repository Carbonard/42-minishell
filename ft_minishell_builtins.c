/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_builtins.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 21:35:56 by rselva-2          #+#    #+#             */
/*   Updated: 2026/03/31 21:41:11 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

// static void	manage_redirections(t_context *ctx, t_redirection *redir, char *here_doc)
// {
// 	;
// }

// static void	close_redirections(t_context *ctx, t_redirection *redir)
// {
// 	;
// }

int	try_builtins(t_context *ctx, char **argv, t_redirection *redir, char *here_doc)
{
	if (ft_strncmp(argv[0], "pwd", 4) && ft_strncmp(argv[0], "env", 4)
		&& ft_strncmp(argv[0], "echo", 5) && ft_strncmp(argv[0], "export", 7)
		&& ft_strncmp(argv[0], "unset", 6) && ft_strncmp(argv[0], "exit", 5)
		&& ft_strncmp(argv[0], "cd", 3))
		return (0);
	(void)redir;
	(void)here_doc;
	// manage_redirections(ctx, redir, here_doc);
	if (!ft_strncmp(argv[0], "pwd", 4))
		ctx->status = pwd();
	else if (!ft_strncmp(argv[0], "env", 4))
		ctx->status = print_env(ctx);
	else if (!ft_strncmp(argv[0], "echo", 5))
		ctx->status = echo(argv);
	else if (!ft_strncmp(argv[0], "export", 7))
		ctx->status = export(ctx, argv[1]);
	else if (!ft_strncmp(argv[0], "unset", 6))
		ctx->status = unset(ctx, argv[1]);
	else if (!ft_strncmp(argv[0], "exit", 5))
		builtin_exit(ctx, argv);
	else if (!ft_strncmp(argv[0], "cd", 3))
		ctx->status = cd(ctx, argv);
	builtins_errors(ctx, argv);
	free_split(argv);
	// close_redirections(ctx, redir);
	ctx->read_exit_status = 1;
	return (1);
}
