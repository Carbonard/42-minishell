/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_builtins.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 21:35:56 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/20 00:12:03 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

int	check_build_ins(t_context *ctx, char **command)
{
	if (!ft_strncmp(command[0], "cd", 3))
		chdir(command[1]);
	else if (!ft_strncmp(command[0], "pwd", 4))
		pwd();
	else if (!ft_strncmp(command[0], "env", 4))
		print_env(ctx);
	else if (!ft_strncmp(command[0], "echo", 5))
		echo(command);
	else if (!ft_strncmp(command[0], "export", 7))
		export(ctx, command[1]);
	else if (!ft_strncmp(command[0], "unset", 6))
		unset(ctx, command[1]);
	else if (!ft_strncmp(command[0], "exit", 5))
		ft_exit(ctx);
	else
		return (MS_BUILT_IN_NFOUND);
	return (MS_BUILT_IN_EXECUTED);
}
