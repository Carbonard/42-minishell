/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_texts.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 22:59:49 by rselva-2          #+#    #+#             */
/*   Updated: 2025/10/12 13:52:30 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_char(char c, t_flags *flags)
{
	size_t	i;

	i = 0;
	while (flags->align == 'r' && i + 1 < flags->width)
	{
		write(FD, " ", 1);
		i++;
	}
	ft_putchar_fd(c, FD);
	while (flags->align == 'l' && i + 1 < flags->width)
	{
		write(FD, " ", 1);
		i++;
	}
	return (1 + i);
}

int	print_str(char *str, t_flags *flags)
{
	size_t	len;
	size_t	i;

	if (str == NULL)
	{
		return (print_str_null(flags));
	}
	len = ft_strlen(str);
	if (flags->precision < 0)
		flags->precision = len;
	i = 0;
	while (flags->align == 'r'
		&& (i + len < flags->width || i + flags->precision < flags->width))
	{
		write(FD, " ", 1);
		i++;
	}
	i += ft_putnstr_fd(str, FD, flags->precision);
	while (flags->align == 'l' && i < flags->width)
	{
		write(FD, " ", 1);
		i++;
	}
	return (i);
}

int	print_str_null(t_flags *flags)
{
	if (flags->precision < 6 && flags->precision >= 0)
	{
		flags->precision = 0;
	}
	return (print_str("(null)", flags));
}
