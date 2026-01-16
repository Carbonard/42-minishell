/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:43:40 by rselva-2          #+#    #+#             */
/*   Updated: 2025/10/03 10:50:44 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	aux_cmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] == s2[i] && s2[i] != 0)
		i++;
	if (s2[i] == 0)
		return (0);
	return (s1[i] - s2[i]);
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	char	*haystack;
	char	*needle;
	size_t	size1;
	size_t	size2;
	size_t	i;

	haystack = (char *)big;
	needle = (char *)little;
	size1 = ft_strlen(haystack);
	size2 = ft_strlen(needle);
	i = 0;
	while ((i < size1 && i + size2 <= len) || (size1 + size2 == 0))
	{
		if (aux_cmp(haystack + i, needle) == 0)
			return (haystack + i);
		i++;
	}
	return (NULL);
}
