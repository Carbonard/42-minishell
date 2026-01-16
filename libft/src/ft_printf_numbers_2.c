/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_numbers_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 23:00:07 by rselva-2          #+#    #+#             */
/*   Updated: 2025/10/12 13:47:07 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_unsigned(unsigned long n, char c, t_flags *flags)
{
	char	*str_num;
	size_t	len;
	size_t	i;

	if (flags->special && (c == 'x' || c == 'X'))
		return (print_special_hex(n, c, flags));
	if (c == 'x')
		str_num = ft_utoa_base(n, "0123456789abcdef");
	else if (c == 'X')
		str_num = ft_utoa_base(n, "0123456789ABCDEF");
	else
		str_num = ft_utoa_base(n, "0123456789");
	len = ft_strlen(str_num);
	i = pre_print_nbr_width(1, flags, len);
	i = pre_print_nbr_precision(1, i, flags, len);
	ft_putstr_fd(str_num, FD);
	i = post_print_nbr(flags, i, len);
	free(str_num);
	return (len + i);
}

int	print_special_hex(unsigned long n, char c, t_flags *flags)
{
	char	*str_num;
	size_t	len;
	size_t	i;

	if (flags->width > 2)
		flags->width -= 2;
	if (c == 'X')
		str_num = ft_utoa_base(n, "0123456789ABCDEF");
	else
		str_num = ft_utoa_base(n, "0123456789abcdef");
	len = ft_strlen(str_num);
	if (flags->zero != '0')
		i = pre_print_nbr_width(1, flags, len);
	if (c == 'X')
		write(FD, "0X", 2);
	else
		write(FD, "0x", 2);
	if (flags->zero == '0')
		i = pre_print_nbr_width(1, flags, len);
	ft_putstr_fd(str_num, FD);
	i = post_print_nbr(flags, i, len);
	free(str_num);
	return (i + len + 2);
}

int	print_pointer(void *ptr, t_flags *flags)
{
	if (ptr == NULL)
		return (print_str("(nil)", flags));
	return (print_special_hex((unsigned long)ptr, 'x', flags));
}
