/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyxssa <nyxssa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 14:57:20 by rselva-2          #+#    #+#             */
/*   Updated: 2026/03/24 17:32:14 by nyxssa           ###   ########.fr       */
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
# include <fcntl.h>
# include <signal.h>

# define MAX_PWD 1024

// int received_signal = 0;

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

enum e_operators
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

enum e_automaton_events
{
	E_SPACE,
	E_SINGLE_QUOTE,
	E_DOUBLE_QUOTE,
	E_AND,
	E_OR,
	E_PIPE,
	E_REDIR_R,
	E_REDIR_L,
	E_DOUBLE_REDIR_R,
	E_DOUBLE_REDIR_L,
	E_OPEN_PAR,
	E_CLOSING_PAR,
	E_OTHER,
	E_TOTAL
};

enum e_automaton_states
{
	S_INITIAL,
	S_COMMAND,
	S_SING_QUOT,
	S_DOUB_QUOT,
	S_OPEN_PAR,
	S_CLOS_PAR,
	S_REDIR,
	S_READ_REDIR,
	S_ERROR,
	S_LAST,
	S_TOTAL
};

typedef struct s_command_tree
{
	struct s_command_tree	*cmd1;
	struct s_command_tree	*cmd2;
	char					*cmd;
	int						sep;
	int						subshell;
	char					*here_doc;
}	t_command_tree;

typedef struct s_redirection
{
	int		type_in;
	char	*file_in;
	int		type_out;
	char	*file_out;
}	t_redirection;

typedef struct s_context
{
	char			*shell_name;
	int				status;
	int				exit_status;
	int				read_exit_status;
	t_str_list		*env;
	char			*user_input;
	int				interactive;
	t_command_tree	cmd_tree;
	int				pipe_fds[2];
	t_dyn_ptr		here_docs;
}	t_context;

int			save_env(t_context *ctx, char **original_env);
void		set_shell(t_context *ctx, char *shell_name);
int			add_env(t_context *ctx, char *new_var);
t_str_list	*find_env_node(t_context *ctx, char *var);
char		*find_env_value(t_context *ctx, char *var);
int			del_env(t_context *ctx, char *var_name);
int			get_prompt(char *prompt);
void		read_input(t_context *ctx);
// Parser
int			find_closing_par(char *str);
void		create_tree(t_command_tree *input);
void		display_tree(t_command_tree *tree);
// Built-ins
// void	exit_builtin(char *cmd);
void		clear_input(t_context *ctx);
int			valid_flag(const char *arg, char f);
int			cd(t_context *ctx, char *new_dir);
int			echo(char **argv);
int			pwd(void);
int			print_env(t_context *ctx);
int			export(t_context *ctx, char *new_var);
int			unset(t_context *ctx, char *var);
void		ft_exit(t_context *ctx, long status);
// Execute commands
char		**split_cmd(char *cmd, t_redirection *redir);
int			try_builtins(t_context *ctx, char **command);
char		*find_cmd_path(t_context *ctx, char *cmd);
char		**list_to_strarray(t_str_list *env);
// int		execute_subshell(t_context *ctx, t_command_tree *node);
void		execute_input(t_context *ctx);
int			execute_leaf(t_context *ctx, t_command_tree *node);
// int		execute_logic(t_context *ctx, t_command_tree *node);
// int		execute_pipe(t_context *ctx, t_command_tree *node);
int			execute_node(t_context *ctx, t_command_tree *node);
// Signals
void		handler_sigint(int sig);
void		do_nothing(int sig);

// Debug
void		display_tree(t_command_tree *tree);

#endif
