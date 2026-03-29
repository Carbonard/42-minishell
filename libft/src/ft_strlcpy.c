/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 18:00:06 by rselva-2          #+#    #+#             */
/*   Updated: 2026/03/28 23:04:35 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// This function does not behave like the original
void	ft_strlcpy(char *dst, const char *src, size_t dsize)
{
	size_t	i;

	if (!dst || !src)
	{
		dst[0] = 0;
		return ;
	}
	if (dsize > 0)
	{
		i = 0;
		while ((i < (dsize - 1)) && (src[i] != 0))
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
}
