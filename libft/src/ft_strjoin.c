/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 20:36:30 by rselva-2          #+#    #+#             */
/*   Updated: 2025/12/04 20:29:14 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*solve_null_error(char const *s1, char const *s2)
{
	if (s1 == NULL && s2 != NULL)
		return (ft_strdup(s2));
	if (s1 != NULL && s2 == NULL)
		return (ft_strdup(s1));
	return (NULL);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*joined;
	size_t	size1;
	size_t	size2;
	size_t	i;

	if (s1 == NULL || s2 == NULL)
		return (solve_null_error(s1, s2));
	size1 = ft_strlen(s1);
	size2 = ft_strlen(s2);
	joined = ft_str_malloc((size1 + size2) * sizeof(char));
	if (joined == NULL)
		return (NULL);
	i = 0;
	while (i < size1)
	{
		joined[i] = s1[i];
		i++;
	}
	i = 0;
	while (i < size2)
	{
		joined[size1 + i] = s2[i];
		i++;
	}
	return (joined);
}
