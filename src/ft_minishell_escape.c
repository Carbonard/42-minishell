/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_escape.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 16:59:48 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/16 21:24:25 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

// length of find must be greater than length of subst
void	str_replace(char *str, char *find, char *subst)
{
	size_t	i;
	size_t	j;
	size_t	len_find;
	size_t	len_subst;

	len_find = ft_strlen(find);
	len_subst = ft_strlen(subst);
	i = 0;
	while (str[i])
	{
		if (!ft_strncmp(str + i, find, len_find))
		{
			j = i;
			while (str[j + len_find])
			{
				str[j + len_subst] = str[j + len_find];
				j++;
			}
			str[j + len_subst] = 0;
			ft_memcpy(str + i, subst, len_subst);
			i += len_subst;
		}
		else
			i++;
	}
}

static void	escape_base(char *str, char *base)
{
	char	new_character[2];
	char	*old_sequence;
	int		i;

	new_character[0] = ft_atoi_base(str + 2, base);
	new_character[1] = 0;
	i = 0;
	while (ft_isdigit(str[i + 2]) && i < 3)
		i++;
	old_sequence = ft_substr(str, 0, 2 + i);
	str_replace(str, old_sequence, new_character);
	free(old_sequence);
}

void	escape_text(char *str)
{
	str_replace(str, "\\a", "\a");
	str_replace(str, "\\n", "\n");
	str_replace(str, "\\t", "\t");
	str_replace(str, "\\r", "\r");
	str_replace(str, "\\e", "\033");
	str_replace(str, "\\[", "\001");
	str_replace(str, "\\]", "\002");
	str_replace(str, "\\\\", "\\");
	str_replace(str, "\\'", "'");
	str_replace(str, "\\\"", "\"");
	size_t	i;
	i = 0;
	while (str[i])
	{
		if (!ft_strncmp(str + i, "\\0", 2))
			escape_base(str + i, "01234567");
		else if (!ft_strncmp(str + i, "\\x", 2))
			escape_base(str + i, "0123456789abcdef");
		i++;
	}
}
