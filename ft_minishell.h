/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 14:57:20 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/19 23:15:27 by rselva-2         ###   ########.fr       */
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

# define MAX_PWD 1024

enum e_status
{
	MS_SUCCESS,
	MS_EXIT,
	MS_BUILT_IN_EXECUTED,
	MS_BUILT_IN_NFOUND,
	MS_E_ENV_MALLOC,
	MS_E_ENV_NFOUND,
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

typedef struct s_command
{
	struct s_command	*cmd1;
	struct s_command	*cmd2;
	char				*cmd;
	int					sep;
	char				*file;
}	t_command;

typedef struct s_redirection
{
	int		type;
	char	*file;
}	t_redirection;

typedef struct s_context
{
	t_str_list		*env;
	char		*user_input;
	t_command	*cmd_tree;
}	t_context;

int	save_env(t_context *ctx, char **original_env);
int	add_env(t_context *ctx, char *new_var);
t_str_list	*find_env_node(t_context *ctx, char *var);
char	*find_env_value(t_context *ctx, char *var);
int	del_env(t_context *ctx, char *var_name);
char	*read_input(void);
// Parser
void	create_tree(t_command *input);
void	display_tree(t_command *tree);
// Built-ins
void	echo(char **argv);
void	pwd(void);
void	print_env(t_context *ctx);
int	export(t_context *ctx, char *new_var);
int	unset(t_context *ctx, char *var);
// Execute commands
int		execute(t_context *ctx, t_command *command);
char	**split_cmd(char *cmd, t_redirection *redir);
int	check_build_ins(t_context *ctx, char **command);
// Debug
void	display_tree(t_command *tree);

#endif
