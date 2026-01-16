/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 19:16:52 by rselva-2          #+#    #+#             */
/*   Updated: 2025/12/03 23:40:20 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	*chr;

	if (!s)
		return (NULL);
	chr = (char *)s;
	while (*chr != (char)c && *chr != '\0')
		chr++;
	if (*chr == '\0' && (char)c != 0)
		return (NULL);
	return (chr);
}
