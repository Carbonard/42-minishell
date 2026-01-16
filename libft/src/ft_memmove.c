/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:23:21 by rselva-2          #+#    #+#             */
/*   Updated: 2025/10/03 11:25:04 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// static void	special_case(char *dst, char *src, size_t n, size_t overlap)
// {
// 	char	tmp1;
// 	char	tmp2;
// 	size_t	i;
// 	size_t	k;

// 	i = 0;
// 	while (i < overlap)
// 	{
// 		k = 0;
// 		tmp1 = src[i];
// 		while (i + k < n)
// 		{
// 			tmp2 = dst[i + k];
// 			dst[i + k] = tmp1;
// 			tmp1 = tmp2;
// 			k += overlap;
// 		}
// 		i++;
// 	}
// }
static void	special_case(char *dst, char *src, size_t n)
{
	while (n-- > 0)
		dst[n] = src[n];
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char	*chr_dst;
	char	*chr_src;

	chr_dst = (char *)dest;
	chr_src = (char *)src;
	if (chr_src < chr_dst)
	{
		special_case(chr_dst, chr_src, n);
		return (dest);
	}
	return (ft_memcpy(dest, src, n));
}

// void	*ft_memmove(void *dest, const void *src, size_t n)
// {
// 	char	*chr_dst;
// 	char	*chr_src;
// 	size_t	i;

// 	chr_dst = (char *)dest;
// 	chr_src = (char *)src;
// 	i = 0;
// 	while (i < n)
// 	{
// 		if (chr_src + i == chr_dst)
// 		{
// 			special_case(chr_dst, chr_src, n);
// 			return (dest);
// 		}
// 		i++;
// 	}
// 	return (ft_memcpy(dest, src, n));
// }
