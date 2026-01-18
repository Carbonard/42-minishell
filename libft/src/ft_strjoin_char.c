/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_char.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyxssa <nyxssa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 16:11:52 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/18 15:58:11 by nyxssa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin_char(char const *s1, char const *s2, char c)
{
	char	*joined;
	size_t	size1;
	size_t	size2;
	size_t	i;

	size1 = ft_strlen(s1);
	size2 = ft_strlen(s2);
	joined = ft_str_malloc((size1 + size2 + 1) * sizeof(char));
	if (joined == NULL)
		return (NULL);
	i = 0;
	while (i < size1)
	{
		joined[i] = s1[i];
		i++;
	}
	joined[i] = c;
	i = 0;
	while (i < size2)
	{
		joined[size1 + i + 1] = s2[i];
		i++;
	}
	return (joined);
}
