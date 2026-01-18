/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyxssa <nyxssa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 14:57:20 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/18 18:56:08 by nyxssa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MINISHELL_H
# define FT_MINISHELL_H

# include "libft.h"
# include <stdio.h>

# define MAX_PWD 1024

enum e_separators
{
	AND,// &&
	OR,// ||
	PIPE,// |
	REDIRECTION_OUT,// >
	REDIRECTION_APP,//>>
	REDIRECTION_IN,// <
	HERE_DOC// <<
};

typedef struct s_command
{
	struct s_command	*cmd1;
	struct s_command	*cmd2;
	char				*cmd;
	int					sep;
}	t_command;

// Parser
void	create_tree(t_command *input);
void	remove_parenthesis(char *str);
void	display_tree(t_command *tree);
// Built-ins
void	echo(char **argv);
void	pwd(void);
void	print_env(char **env);

#endif
