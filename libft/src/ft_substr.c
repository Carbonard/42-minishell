/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 20:42:19 by rselva-2          #+#    #+#             */
/*   Updated: 2025/12/04 20:34:21 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	i;
	size_t	s_len;

	i = 0;
	while (i < start)
	{
		if (s[i] == 0)
			return (ft_calloc(1, sizeof(char)));
		i++;
	}
	s_len = ft_strlen(s);
	if (start + len < s_len)
		substr = ft_str_malloc(len * sizeof(char));
	else
		substr = ft_str_malloc((s_len - start) * sizeof(char));
	if (substr == NULL)
		return (NULL);
	i = 0;
	while (i < len && start + i < s_len)
	{
		substr[i] = s[start + i];
		i++;
	}
	return (substr);
}
