/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_flags.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 23:00:17 by rselva-2          #+#    #+#             */
/*   Updated: 2025/10/12 15:25:51 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	reset_flags(t_flags *flags)
{
	flags->zero = ' ';
	flags->align = 'r';
	flags->precision = -1;
	flags->width = 1;
	flags->plus = '-';
	flags->special = 0;
}

static int	read_int(const char *str, int i, va_list *args, int *n)
{
	if (str[i] == '*')
	{
		*n = va_arg(*args, int);
		i++;
	}
	else
	{
		*n = atoi(str + i);
		while (str[i] == '0')
			i++;
		i += intlen(*n);
	}
	return (i);
}

int	read_flags(const char *str, int i, va_list *args, t_flags *flags)
{
	int	n;

	while (str[i] == '-' || str[i] == '0' || str[i] == ' ' || str[i] == '+'
		|| str[i] == '#')
	{
		set_flag(str[i], flags);
		i++;
	}
	i = read_int(str, i, args, &n);
	flags->width = n;
	if (str[i] == '.')
	{
		i++;
		i = read_int(str, i, args, &n);
		flags->precision = n;
		flags->zero = ' ';
	}
	while (str[i] == '0')
		i++;
	return (i);
}

void	set_flag(char f, t_flags *flags)
{
	if (f == '-')
		flags->align = 'l';
	else if (f == '0')
		flags->zero = '0';
	else if (f == ' ' && flags->plus != '+')
		flags->plus = ' ';
	else if (f == '+')
		flags->plus = '+';
	else if (f == '#')
		flags->special = 1;
}
