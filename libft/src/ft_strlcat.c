/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 18:52:39 by rselva-2          #+#    #+#             */
/*   Updated: 2026/03/28 22:52:44 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	size_dst;
	size_t	size_src;
	size_t	i_dst;
	size_t	i_src;

	if (!src)
		return (0);
	size_dst = ft_strlen(dst);
	if (size < size_dst)
		size_dst = size;
	size_src = ft_strlen(src);
	i_src = 0;
	i_dst = size_dst;
	if (size > 0)
	{
		while (i_dst < size - 1 && src[i_src] != 0)
		{
			dst[i_dst] = src[i_src];
			i_dst++;
			i_src++;
		}
	}
	if (i_dst < size)
		dst[i_dst] = '\0';
	return (size_dst + size_src);
}
