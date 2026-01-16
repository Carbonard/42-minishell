/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 23:00:39 by rselva-2          #+#    #+#             */
/*   Updated: 2025/11/24 15:48:57 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include "libft.h"
# include <stdarg.h>
# define FD 2

typedef struct s_flags
{
	char	zero;
	char	align;
	long	precision;
	size_t	width;
	char	plus;
	int		special;
}	t_flags;

// ft_printf_prints.c
int		print_char(char c, t_flags *flags);
int		print_str(char *str, t_flags *flags);
int		print_str_null(t_flags *flags);
// ft_printf_numbers_1.c
int		intlen(long long n);
size_t	pre_print_nbr_width(int n, t_flags *flags, size_t len);
size_t	pre_print_nbr_precision(int n, size_t i, t_flags *flags, size_t len);
size_t	post_print_nbr(t_flags *flags, size_t i, size_t len);
int		print_int(int n, t_flags *flags);
// ft_printf_numbers_2.c
int		print_unsigned(unsigned long n, char c, t_flags *flags);
int		print_special_hex(unsigned long n, char c, t_flags *flags);
int		print_pointer(void *ptr, t_flags *flags);
// ft_printf_flags
void	reset_flags(t_flags *flags);
int		read_flags(const char *str, int i, va_list *args, t_flags *flags);
void	set_flag(char f, t_flags *flags);
// ft_printf.c
int		ft_printf(const char *str, ...);

#endif