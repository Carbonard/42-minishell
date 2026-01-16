/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 01:35:55 by rselva-2          #+#    #+#             */
/*   Updated: 2025/10/15 22:52:32 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	base_len(const char *base)
{
	int	len;
	int	i;
	int	j;

	len = 0;
	while (base[len] != '\0')
	{
		if (base[len] == '+' || base[len] == '-')
			return (-1);
		len ++;
	}
	i = -1;
	while (++i < len)
	{
		j = i;
		while (++j < len)
			if (base[i] == base[j])
				return (-1);
	}
	return (len);
}

static char	*utoa(unsigned long n, const char *base, int b)
{
	char	arr[100];
	char	*str;
	int		i;
	int		len;

	len = 0;
	while (n > 0)
	{
		arr[len] = *(base + n % b);
		n = n / b;
		len++;
	}
	str = malloc((len + 1) * sizeof(char));
	str[len] = 0;
	i = len;
	while (i > 0)
	{
		i--;
		str[i] = arr[len - 1 - i];
	}
	return (str);
}

char	*ft_utoa_base(unsigned long nbr, const char *base)
{
	int	base_length;

	base_length = base_len(base);
	if (base_length <= 1)
		return (NULL);
	return (utoa((unsigned long) nbr, base, base_length));
}
