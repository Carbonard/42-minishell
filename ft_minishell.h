/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyxssa <nyxssa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 14:57:20 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/19 22:13:36 by nyxssa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MINISHELL_H
# define FT_MINISHELL_H

# include "libft.h"
# include "dynamic_arrays/src/ft_dynarray.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

# define MAX_PWD 1024

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
	char		*user_input;
	t_command	*cmd_tree;
}	t_context;

char	*read_input(void);
// Parser
void	create_tree(t_command *input);
void	display_tree(t_command *tree);
// Built-ins
void	echo(char **argv);
void	pwd(void);
void	print_env(char **env);
// Execute commands
int		execute(t_command *command, char **env);
char	**split_cmd(char *cmd, t_redirection *redir);
int		check_build_ins(char **command, char **env);
// Debug
void	display_tree(t_command *tree);

#endif
