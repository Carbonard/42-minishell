/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_malloc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 16:04:53 by rselva-2          #+#    #+#             */
/*   Updated: 2025/12/04 20:03:47 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_str_malloc(size_t size)
{
	char	*str;

	str = malloc(size + 1);
	if (str == NULL)
		return (NULL);
	str[size] = 0;
	return (str);
}
