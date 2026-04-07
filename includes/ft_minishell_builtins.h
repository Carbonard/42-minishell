/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_builtins.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 18:16:41 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/03 19:07:23 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

// environment
int			save_env(t_context *ctx, char **original_env);
int			add_env(t_context *ctx, char *new_var);
t_str_list	*find_env_node(t_context *ctx, char *var);
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
// error_messages
void		builtins_errors(t_context *ctx, char **argv);
