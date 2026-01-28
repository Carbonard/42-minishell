/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 14:57:20 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/28 14:32:54 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MINISHELL_H
# define FT_MINISHELL_H

# include "libft/libft.h"
# include "dynamic_arrays/src/ft_dynarray.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <sys/wait.h>

# define MAX_PWD 1024

enum e_status
{
	MS_SUCCESS,
	MS_OPENQUOTE,
	MS_OPENPAR,
	MS_OPENOP,
	MS_E_SYNTAX,
	MS_E_SYNTAX_EOF,
	MS_EXIT,
	MS_BUILT_IN_EXECUTED,
	MS_BUILT_IN_NFOUND,
	MS_E_PWD_NFOUND,
	MS_E_ENV_MALLOC,
	MS_E_ENV_NFOUND,
	MS_E_VAR_NFOUND,
	MS_E_EQINNAME,
	MS_E_PATH_NFOUND
};

enum e_BUILTINS
{
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT
};

enum e_separators
{
	NONE,
	AND,
	OR,
	PIPE,
	REDIRECTION_OUT,
	REDIRECTION_APP,
	REDIRECTION_IN,
	HERE_DOC
};

typedef struct s_command_tree
{
	struct s_command_tree	*cmd1;
	struct s_command_tree	*cmd2;
	char				*cmd;
	int					sep;
	int					subshell;
}	t_command_tree;

typedef struct s_redirection
{
	int		type;
	char	*file;
}	t_redirection;

typedef struct s_context
{
	int				status;
	int				exit_status;
	int				read_exit_status;
	t_str_list		*env;
	char			*user_input;
	int				interactive;
	t_command_tree	cmd_tree;
	int				pipe_fds[2];
}	t_context;

int		save_env(t_context *ctx, char **original_env);
int		add_env(t_context *ctx, char *new_var);
t_str_list	*find_env_node(t_context *ctx, char *var);
char	*find_env_value(t_context *ctx, char *var);
int		del_env(t_context *ctx, char *var_name);
void	read_input(t_context *ctx);
// Parser
int		find_closing_par(char *str);
void	create_tree(t_command_tree *input);
void	display_tree(t_command_tree *tree);
// Built-ins
// void	exit_builtin(char *cmd);
void	clear_input(t_context *ctx);
int		valid_flag(const char *arg, char f);
int		echo(char **argv);
int		pwd(void);
int		print_env(t_context *ctx);
int		export(t_context *ctx, char *new_var);
int		unset(t_context *ctx, char *var);
void	ft_exit(t_context *ctx);
// Execute commands
char	**split_cmd(char *cmd, t_redirection *redir);
void	try_builtins(t_context *ctx, char **command);
char	*find_cmd_path(t_context *ctx, char *cmd);
char	**list_to_strarray(t_str_list *env);
int		execute_subshell(t_context *ctx, t_command_tree *node);
void	execute_command(t_context *ctx, char **argv);
int		execute_leaf(t_context *ctx, t_command_tree *node);
int		execute_logic(t_context *ctx, t_command_tree *node);
int		execute_pipe(t_context *ctx, t_command_tree *node);
int		execute_node(t_context *ctx, t_command_tree *node);
void	execute_input(t_context *ctx);

// Debug
void	display_tree(t_command_tree *tree);

#endif
