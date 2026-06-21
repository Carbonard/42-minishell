/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 18:00:06 by rselva-2          #+#    #+#             */
/*   Updated: 2026/06/21 16:22:00 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// This function may not behave exactly like the original
int	ft_strlcpy(char *dst, const char *src, size_t dsize)
{
	size_t	i;

	i = 0;
	if (!dst || !src)
	{
		dst[0] = 0;
		return (0);
	}
	if (dsize > 0)
	{
		while ((i < (dsize - 1)) && (src[i] != 0))
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (i);
}
