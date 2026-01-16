/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 23:00:14 by rselva-2          #+#    #+#             */
/*   Updated: 2025/11/27 23:55:24 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	write_conversion(char c, t_flags *flags,
	va_list *args, int cnt)
{
	if (c == '%')
	{
		cnt += 1;
		ft_putchar_fd('%', FD);
	}
	else if (c == 'c')
		cnt += print_char(va_arg(*args, int), flags);
	else if (c == 's')
		cnt += print_str(va_arg(*args, char *), flags);
	else if (c == 'i' || c == 'd')
		cnt += print_int(va_arg(*args, int), flags);
	else if (c == 'u' || c == 'x' || c == 'X')
		cnt += print_unsigned(va_arg(*args, unsigned int), c, flags);
	else if (c == 'u' || c == 'p')
		cnt += print_pointer(va_arg(*args, void *), flags);
	return (cnt);
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	t_flags	*flags;
	size_t	i;
	int		cnt;

	va_start(args, str);
	flags = malloc(sizeof(t_flags));
	cnt = 0;
	i = 0;
	while (str[i] != 0)
	{
		reset_flags(flags);
		if (str[i] != '%')
			cnt += print_char(*(str + i), flags);
		else
		{
			i = read_flags(str, i + 1, &args, flags);
			cnt = write_conversion(str[i], flags, &args, cnt);
		}
		i++;
	}
	va_end(args);
	free(flags);
	return (cnt);
}
