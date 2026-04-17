/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_execution.h                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 18:11:57 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/17 14:52:23 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

typedef struct s_exec_args
{
	char		exec_args[ARG_MAX];
	char		*path;
	char		*static_argv[ARG_MAX];
	char		*env[ARG_MAX];
	size_t		args_length;
}	t_exec_args;

// parser
void	create_tree(t_command_tree *input);
// execute_tree
int		get_status(t_context *ctx, int wstatus);
void	execute_input(t_context *ctx);
// execute_node
int		execute_node(t_context *ctx, t_command_tree *node);
// manage_leaf
void	manage_redirection(t_context *ctx, t_redirection *redir, char *heredoc);
int		execute_leaf(t_context *ctx, t_command_tree *node);
// argv
char	**get_argv_and_redir(t_context *ctx, char *cmd, t_redirection *redir);
char	*expand_input(t_context *ctx, char *input);
// split_cmd
int		advance_quotes(char *str);
char	**split_cmd(char *cmd, t_redirection *redir);
// wildcards
char	*expand_wildcards(char *cmd);
void	sort_entries(char **arr);
// builtins
int		try_builtins(t_context *ctx, char **argv, t_redirection *redir,
			char *heredoc);
// execute_command
int		find_cmd_path(t_context *ctx, char *path, char *cmd);
void	execute_command(t_context *ctx, char **argv);