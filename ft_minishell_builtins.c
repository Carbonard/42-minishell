/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_builtins.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 21:35:56 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/17 21:39:16 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

int	check_build_ins(char **command, char **env)
{
	if (!ft_strncmp(command[0], "cd", 3))
		chdir(command[1]);
	else if (!ft_strncmp(command[0], "pwd", 4))
		pwd();
	else if (!ft_strncmp(command[0], "env", 4))
		print_env(env);
	else if (!ft_strncmp(command[0], "echo", 7))
		echo(command);
}
