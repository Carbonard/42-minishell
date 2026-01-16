/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_numbers_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 23:00:07 by rselva-2          #+#    #+#             */
/*   Updated: 2025/10/12 14:41:02 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	intlen(long long num)
{
	int	len;

	len = 0;
	if (num < 0)
	{
		num = -num;
	}
	while (num > 0)
	{
		num /= 10;
		len++;
	}
	return (len);
}

size_t	pre_print_nbr_width(int sgn, t_flags *flags, size_t len)
{
	size_t	i;
	size_t	j;

	if (flags->precision < 0)
		flags->precision = 1;
	i = 0;
	if (flags->zero == '0' && (sgn < 0 || (sgn > 0 && flags->plus != '-')))
	{
		if (sgn < 0)
			ft_putchar_fd('-', FD);
		else if (flags->plus != '-')
			ft_putchar_fd(flags->plus, FD);
		i++;
	}
	j = 0;
	if ((sgn < 0 || (sgn > 0 && flags->plus != '-')) && flags->zero != '0')
		j = 1;
	while (flags->align == 'r' && i + j + len < flags->width
		&& i + j + flags->precision < flags->width)
	{
		ft_putchar_fd(flags->zero, FD);
		i++;
	}
	return (i);
}

size_t	pre_print_nbr_precision(int sgn, size_t i, t_flags *flags, size_t len)
{
	size_t	j;

	if (flags->zero != '0' && (sgn < 0 || (sgn > 0 && flags->plus != '-')))
	{
		if (sgn < 0)
			ft_putchar_fd('-', FD);
		else if (flags->plus != '-')
			ft_putchar_fd(flags->plus, FD);
		i++;
	}
	j = 0;
	while (j + len < (size_t)flags->precision)
	{
		ft_putchar_fd('0', FD);
		j++;
		i++;
	}
	return (i);
}

size_t	post_print_nbr(t_flags *flags, size_t i, size_t len)
{
	while (flags->align == 'l' && i + len < flags->width)
	{
		ft_putchar_fd(' ', FD);
		i++;
	}
	return (i);
}

int	print_int(int n, t_flags *flags)
{
	size_t	len;
	size_t	i;
	char	*str_num;

	len = intlen(n);
	i = pre_print_nbr_width(n, flags, len);
	i = pre_print_nbr_precision(n, i, flags, len);
	if (n < 0)
		str_num = ft_utoa_base(-(long)n, "0123456789");
	else
		str_num = ft_utoa_base(n, "0123456789");
	ft_putstr_fd(str_num, FD);
	free (str_num);
	i = post_print_nbr(flags, i, len);
	return (len + i);
}
