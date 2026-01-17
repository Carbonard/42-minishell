/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 14:57:20 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/17 15:33:09 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MINISHELL_H
# define FT_MINISHELL_H

# include "libft.h"
# include <stdio.h>
// # include <readline.h>

# define MAX_PWD 1024

typedef struct s_command
{
	struct s_command	*cmd1;
	struct s_command	*cmd2;
	char				*cmd;
	char				sep;
}	t_command;

// Built-ins
void	echo(char **argv);
void	pwd(void);
void	print_env(char **env);

#endif
