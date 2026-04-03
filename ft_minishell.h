/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 14:57:20 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/03 15:44:58 by rselva-2         ###   ########.fr       */
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
# include <dirent.h>

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

# define MAX_PROMPT PATH_MAX + 30

# ifndef PATH_MAX
	#  define PATH_MAX 4096
# endif

// # define ARG_MAX 2097152

// #define ARG_MAX 3605 + 12 + 20

# ifndef ARG_MAX
#  ifdef _POSIX_ARG_MAX
#   define ARG_MAX _POSIX_ARG_MAX
#  else
#   define ARG_MAX 131072
#  endif
# endif

#define INVALID_EXPORT_CHARS "*@#?+-$!"


extern int last_signal;

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
	MS_E_PATH_NFOUND,
	MS_TOO_MANY_ARGS,
	MS_OLDPWD_NOT_SET,
	MS_NON_NUMERIC_ARG,
	MS_SE_QUOTES,
	MS_SE_EOF,
	MS_LONG_PATH,
	MS_LONG_ARGS,
	MS_NOT_VALID_ID
};

enum e_exit_status
{
	ES_SUCCESS = 0,
	ES_CMD_NOT_EXEC = 126,
	ES_CMD_NOT_FOUND = 127,
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

typedef struct s_automaton_data
{
	int		state;
	int		last_state;
	int		event;
	int		i_saved;
	char	saved_states[1000];
}	t_automaton_data;

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
	int		original_in;
	int		original_out;
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

typedef struct s_exec_args
{
	char 		exec_args[ARG_MAX];
	char		*path;
	char		*static_argv[20];
	char		*env[100];
	size_t		args_length;
}	t_exec_args;

// Signals
void		handler_sigint(int sig);
// heredoc
void		read_here_docs(t_context *ctx);
int			spread_here_docs(t_command_tree *node, t_dyn_ptr *hd, int n);
int			is_metachar(char c);
// get_input
int			read_input(t_context *ctx);
// check_syntax
int			check_syntax(char *input);
// check_input
int			check_quotes(t_context *ctx);
int			check_parenthesis(t_context *ctx);
int			is_redirection(char *str);
int			check_operator(t_context *ctx);
// parser
void		create_tree(t_command_tree *input);
// execute_tree
int			get_status(t_context *ctx, int wstatus);
void		execute_input(t_context *ctx);
// execute_node
int			execute_node(t_context *ctx, t_command_tree *node);
// execute_command
int			find_cmd_path(t_context *ctx, char *path, char *cmd);
void		execute_command(t_context *ctx, char **argv);
// manage_leaf
void		manage_redirection(t_context *ctx, t_redirection *redir, char *here_doc);
int			execute_leaf(t_context *ctx, t_command_tree *node);
// argv
char		**get_argv_and_redir(t_context *ctx, char *cmd, t_redirection *redir);
// split_cmd
int			advance_quotes(char *str);
char		**split_cmd(char *cmd, t_redirection *redir);
// wildcards
char		*expand_wildcards(char *cmd);
// builtins
int			try_builtins(t_context *ctx, char **argv, t_redirection *redir, char *here_doc);
// environment
int			save_env(t_context *ctx, char **original_env);
int			add_env(t_context *ctx, char *new_var);
t_str_list	*find_env_node(t_context *ctx, char *var);
char		*find_env_value(t_context *ctx, char *var);
int			del_env(t_context *ctx, char *var_name);
// builtins_env
int			print_env(t_context *ctx);
int			export(t_context *ctx, char *var);
int			builtin_export(t_context *ctx, char **argv);
int			unset(t_context *ctx, char *var);
// builtins_others
int			valid_flag(const char *arg, char f);
int			cd(t_context *ctx, char **argv);
int			echo(char **argv);
int			pwd(void);
// exit
void		ft_exit(t_context *ctx, long status);
void		silent_exit(t_context *ctx, long status);
void		builtin_exit(t_context *ctx, char **argv);
// clears
void		clear_input(t_context *ctx);
void		free_all(t_context *ctx);
// error_messages
void		shell_perror(t_context *ctx, char *s);
void		custom_error(char *s, char *error_msg);
void		builtins_errors(t_context *ctx, char **argv);

// Debug
void		display_tree(t_command_tree *tree);


#endif
