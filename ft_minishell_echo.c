/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_echo.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 16:52:14 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/18 20:48:32 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

static int	valid_flag(const char *arg)
{
	int	i;

	i = 1;
	if (!arg[i])
		return (0);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	echo(char **argv)
{
	int	arg_i;
	int	new_line;

	new_line = 1;
	arg_i = 1;
	while (argv[arg_i] && argv[arg_i][0] == '-')
	{
		if (valid_flag(argv[arg_i]))
			new_line = 0;
		else
			break ;
		arg_i++;
	}
	while (argv[arg_i])
	{
		ft_putstr_fd(argv[arg_i], 1);
		arg_i++;
		if (argv[arg_i])
			ft_putchar_fd(' ', 1);
	}
	if (new_line)
		ft_putchar_fd('\n', 1);
}
