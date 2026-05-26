/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_builtins_echo.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 17:57:25 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/26 01:05:08 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_builtins.h"
#include "ft_minishell_input.h"

static int	is_valid_echo_flag(char *flag)
{
	int	i;

	i = 1;
	while (flag[i])
	{
		if (!ft_strchr(ECHO_FLAGS, flag[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	read_flags(char **argv, t_echo_flags *flags)
{
	int	arg_i;

	flags->no_new_line = 0;
	flags->format = 0;
	arg_i = 1;
	while (argv[arg_i] && argv[arg_i][0] == '-')
	{
		if (!is_valid_echo_flag(argv[arg_i]))
			break ;
		if (ft_strchr(argv[arg_i], 'n'))
			flags->no_new_line = 1;
		if (ft_strchr(argv[arg_i], 'e'))
			flags->format = 1;
		arg_i++;
	}
	return (arg_i);
}

int	echo(char **argv)
{
	int				arg_i;
	t_echo_flags	flags;
// fprintf(stderr, "echoing '%s'\n", argv[1]);
	arg_i = read_flags(argv, &flags);
	while (argv[arg_i])
	{
		if (flags.format)
			escape_text(argv[arg_i]);
		ft_putstr_fd(argv[arg_i], 1);
		arg_i++;
		if (argv[arg_i])
			ft_putchar_fd(' ', 1);
	}
	if (!flags.no_new_line)
		ft_putchar_fd('\n', 1);
	return (MS_SUCCESS);
}
