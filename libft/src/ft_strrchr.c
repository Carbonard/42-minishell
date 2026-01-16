/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 19:17:16 by rselva-2          #+#    #+#             */
/*   Updated: 2025/09/30 19:38:55 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*chr;

	chr = (char *)s;
	while (*chr != '\0')
		chr++;
	while (*chr != (char)c && chr != (char *)s)
		chr--;
	if (chr == (char *)s && (char)c != *chr)
		return (NULL);
	return (chr);
}
