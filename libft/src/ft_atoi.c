/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 16:00:14 by rselva-2          #+#    #+#             */
/*   Updated: 2025/11/30 11:21:51 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*skip_spaces(char *str)
{
	while (*str == ' ' || *str == '\n' || *str == '\t' || *str == '\v'
		|| *str == '\f' || *str == '\r')
		str++;
	return (str);
}

int	ft_atoi(const char *nptr)
{
	char	*ptr;
	long	num;
	int		sgn;
	int		i;

	if (!nptr)
		return (0);
	ptr = skip_spaces((char *) nptr);
	i = 0;
	sgn = 1;
	if (ptr[i] == '-')
		sgn = -1;
	if (ptr[i] == '+' || ptr[i] == '-')
		i++;
	num = 0;
	while (ptr[i] >= '0' && ptr[i] <= '9')
	{
		num = num * 10 + ptr[i] - '0';
		i++;
	}
	return ((int)(num * sgn));
}
