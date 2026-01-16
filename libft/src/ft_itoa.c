/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 20:25:41 by rselva-2          #+#    #+#             */
/*   Updated: 2025/11/30 16:04:43 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned char	length_of_long(long num)
{
	unsigned char	len;

	len = 1;
	while (num > 9)
	{
		len++;
		num /= 10;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char			*nstr;
	unsigned char	len;
	long			num;
	int				is_neg;

	is_neg = (n < 0);
	num = (1 - 2 * is_neg) * (long)n;
	len = length_of_long(num);
	nstr = malloc((len + is_neg + 1) * sizeof(char));
	if (!nstr)
		return (NULL);
	nstr[0] = '-';
	nstr[len + is_neg - 1] = '0';
	nstr[len + is_neg] = 0;
	while (len > 0)
	{
		len--;
		nstr[len + is_neg] = '0' + num % 10;
		num /= 10;
	}
	return (nstr);
}

// One-function version:
// char	*ft_itoa(int n)
// {
// 	char	*nstr;
// 	size_t	len;
// 	long	num;

// 	len = 1;
// 	num = (long)n;
// 	while (num > 9 || num < -9)
// 	{
// 		len++;
// 		num /= 10;
// 	}
// 	nstr = malloc((len + (n < 0) + 1) * sizeof(char));
// 	if (!nstr)
// 		return (NULL);
// 	nstr[0] = '-';
// 	nstr[len + (n < 0) - 1] = '0';
// 	nstr[len + (n < 0)] = 0;
// 	while (len > 0)
// 	{
// 		len--;
// 		nstr[len + (n < 0)] = '0' + (1 - 2 * (n < 0)) * (n % 10);
// 		n /= 10;
// 	}
// 	return (nstr);
// }
