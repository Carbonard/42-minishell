/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 14:57:20 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/30 21:45:44 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MINISHELL_H
# define FT_MINISHELL_H

# include "../libft/libft.h"
# include "../dynamic_arrays/src/ft_dynarray.h"
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

# ifndef ARG_MAX
#  define ARG_MAX 131072
# endif

# define INVALID_EXPORT_CHARS "*@#?+-$!"

extern int	g_last_signal;

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
	MS_NOT_VALID_ID,
	MS_BADFILE,
	MS_CMD_IS_DIR,
	MS_HOME_NOT_SET
};

enum e_exit_status
{
	ES_SUCCESS = 0,
	ES_CMD_NOT_EXEC = 126,
	ES_CMD_NOT_FOUND = 127,
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

typedef struct s_redirection
{
	t_dyn_int	type_in;
	t_dyn_ptr	file_in;
	t_dyn_int	type_out;
	t_dyn_ptr	file_out;
	int			original_in;
	int			original_out;
}	t_redirection;

typedef struct s_command_tree
{
	struct s_command_tree	*cmd1;
	struct s_command_tree	*cmd2;
	char					*cmd;
	int						sep;
	int						subshell;
	char					*here_doc;
	char					*eof;
	char					*redirections;
	t_redirection			redir;
}	t_command_tree;

typedef struct s_context
{
	char			*shell_name;
	int				status;
	int				exit_status;
	int				read_exit_status;
	t_str_list		*env;
	char			*user_input;
	int				no_tty;
	t_command_tree	cmd_tree;
	int				pipe_fds[2];
	t_dyn_ptr		here_docs;
	t_dyn_ptr		eofs;
}	t_context;

// init_config
void	check_interactive(t_context *ctx, int argc, char **argv);
void	set_shell(t_context *ctx, char *shell_name);
// signals
void	rl_handler_sigint(int sig);
void	generic_handler_sigint(int sig);
// history
void	add_input_history(t_context *ctx);
// environment
char	*find_env_value(t_context *ctx, char *var);
// exit
void	ft_exit(t_context *ctx, long status);
void	silent_exit(t_context *ctx, long status);
void	builtin_exit(t_context *ctx, char **argv);
// clears
void	clear_input(t_context *ctx);
void	free_all(t_context *ctx);
// error_messages
void	shell_perror(t_context *ctx, char *s);
void	custom_error(char *s, char *error_msg);
// utils
int		is_metachar(char c);
int		is_redirection(char *str);

// Debug
void	display_tree(t_command_tree *tree);

#endif
