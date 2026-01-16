/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:17:23 by rselva-2          #+#    #+#             */
/*   Updated: 2025/12/04 20:37:26 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	char_in_str(const char c, const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	long	i;
	long	start;
	long	end;
	char	*trimmed;

	if (!s1)
		return (NULL);
	start = 0;
	while (char_in_str(s1[start], set))
		start++;
	end = ft_strlen(s1) - 1;
	while (end > 0 && char_in_str(s1[end], set))
		end--;
	if (start > end)
		return (ft_calloc(1, sizeof(char)));
	trimmed = ft_str_malloc((end - start + 1) * sizeof(char));
	if (trimmed == NULL)
		return (NULL);
	i = start;
	while (i <= end)
	{
		trimmed[i - start] = s1[i];
		i++;
	}
	return (trimmed);
}
