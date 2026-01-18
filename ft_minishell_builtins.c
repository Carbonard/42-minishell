/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_builtins.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 21:35:56 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/18 20:36:31 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

int	check_build_ins(char **command, char **env)
{
	if (!ft_strncmp(command[0], "cd", 3))
	{
		chdir(command[1]);
		return (CD);
	}
	else if (!ft_strncmp(command[0], "pwd", 4))
	{
		pwd();
		return (PWD);
	}
	else if (!ft_strncmp(command[0], "env", 4))
	{
		print_env(env);
		return (ENV);
	}
	else if (!ft_strncmp(command[0], "echo", 7))
	{
		echo(command);
		return (ECHO);
	}
	else if (!ft_strncmp(command[0], "exit", 7))
		return (EXIT);
	else
		return (-1);
	return (-1);
}
