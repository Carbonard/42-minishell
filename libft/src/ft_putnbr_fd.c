/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 09:46:19 by rselva-2          #+#    #+#             */
/*   Updated: 2025/10/02 09:46:20 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	long			num;
	char			number[20];
	unsigned int	i;

	num = (long)n;
	if (num < 0)
	{
		num = -num;
		write(fd, "-", 1);
	}
	else if (num == 0)
		write(fd, "0", 1);
	i = 0;
	while (num > 0)
	{
		number[i] = num % 10 + '0';
		num /= 10;
		i++;
	}
	while (i-- > 0)
		write(fd, number + i, 1);
}
