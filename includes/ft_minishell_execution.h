/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_execution.h                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 18:11:57 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/29 21:03:16 by rselva-2         ###   ########.fr       */
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
int		reduce_index(char *input, int i);
// execute_tree
int		get_status(t_context *ctx, int wstatus);
void	execute_input(t_context *ctx);
// execute_node
int		execute_node(t_context *ctx, t_command_tree *node);
// manage_leaf
int		manage_redirection(t_context *ctx, t_str_list *token, int hd_fd);
int		execute_leaf(t_context *ctx, t_command_tree *node);
// argv
char	**get_argv_and_redir(t_context *ctx, char *cmd);
void	restore_redirections(t_context *ctx);
void	close_redirections(t_context *ctx);
char	*expand_input(t_context *ctx, char *input);
// split_cmd
int		advance_quotes(char *str);
// char	**split_cmd(char *cmd, t_redirection *redir);
// wildcards
int		expand_wildcards(char *token, t_dyn_ptr *argv);
void	sort_entries(char **arr);
// builtins
int		try_builtins(t_context *ctx, char **argv);
// execute_command
int		find_cmd_path(t_context *ctx, char *path, char *cmd);
void	execute_command(t_context *ctx, char **argv);
// tree_utils
int		find_closing_par(char *str);
int		find_open_par(char *str, int i);
int		find_open_quote(char *str, int i);
int		remove_parenthesis(char *str);
// utils
char	*remove_quotes(char *cmd);
