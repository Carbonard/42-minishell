/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 21:11:13 by rselva-2          #+#    #+#             */
/*   Updated: 2025/11/30 16:44:17 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	write(fd, s, ft_strlen(s));
}

size_t	ft_putnstr_fd(char *s, int fd, size_t n)
{
	size_t	i;

	i = 0;
	while (*s != '\0' && i < n)
	{
		write(fd, s, 1);
		s++;
		i++;
	}
	return (i);
}
