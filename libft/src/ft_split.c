/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:01:31 by rselva-2          #+#    #+#             */
/*   Updated: 2025/12/04 20:39:06 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*free_split(char **spl)
{
	size_t	i;

	if (!spl)
	{
		return (NULL);
	}
	i = 0;
	while (spl[i])
	{
		free(spl[i]);
		i++;
	}
	free(spl);
	return (NULL);
}

static size_t	count_words(char const *s, char c)
{
	size_t	count;

	count = 1;
	while (*s)
	{
		if (*(s) != c && (*(s + 1) == c || *(s + 1) == 0))
			count++;
		s++;
	}
	return (count);
}

static char	**save_words(char const *s, char c, char **spl)
{
	size_t	words;
	size_t	length;

	words = 0;
	while (*s)
	{
		if (*s != c)
		{
			length = 0;
			while (s[length] && s[length] != c)
				length++;
			spl[words] = malloc((length + 1) * sizeof(char));
			if (!spl[words])
				return(free_split(spl));
			ft_strlcpy(spl[words], s, length + 1);
			words++;
			s += length;
		}
		else
			s++;
	}
	spl[words] = NULL;
	return (spl);
}

char	**ft_split(char const *s, char c)
{
	char	**spl;

	if (!s)
		return (NULL);
	spl = malloc(count_words(s, c) * sizeof(char *));
	if (!spl)
		return (NULL);
	spl = save_words(s,c, spl);
	return (spl);
}
