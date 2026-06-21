/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_execution.h                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 18:11:57 by rselva-2          #+#    #+#             */
/*   Updated: 2026/06/21 17:14:21 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

typedef struct s_exec_args
{
	char		*path;
	t_dyn_ptr	env;
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
char	*expand_variables(t_context *ctx, char *input);
char	*expand_variable(t_context *ctx, char *str, int *i);
// split_cmd
int		advance_quotes(char *str);
// char	**split_cmd(char *cmd, t_redirection *redir);
// wildcards
int		expand_wildcards(char *token, t_dyn_ptr *argv);
void	sort_entries(char **arr);
// builtins
int		try_builtins(t_context *ctx, char **argv);
// execute_command
char	*find_cmd_path(t_context *ctx, char *cmd);
void	execute_command(t_context *ctx, char **argv);
// tree_utils
int		find_closing_par(char *str);
int		find_open_par(char *str, int i);
int		find_open_quote(char *str, int i);
int		remove_parenthesis(char *str);
// utils
char	*remove_quotes(char *cmd);
// custom_functions
int		custom_fork(t_context *ctx);
int		custom_waitpid(int pid, int *wstatus, int options);
int		custom_dup2(t_context *ctx, int fd1, int fd2);
int		custom_pipe(t_context *ctx, int fds[2]);

char	*triple_join(char *first, char *second, char *third);
void	free_almost_all(t_context *ctx);
