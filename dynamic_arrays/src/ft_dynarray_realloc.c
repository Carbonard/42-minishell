/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dynarray_realloc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 20:10:24 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/21 23:13:39 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dynarray.h"

int	ft_realloc(void **ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	i;

	new_ptr = malloc(new_size);
	if (!new_ptr)
	{
		return (0);
	}
	i = 0;
	while (i < old_size && i < new_size)
	{
		((unsigned char *)new_ptr)[i] = ((unsigned char *)(*ptr))[i];
		i++;
	}
	free(*ptr);
	*ptr = new_ptr;
	return (1);
}

int	ft_realloc2(void ***ptr, size_t old_size, size_t new_size)
{
	void	**new_ptr;
	size_t	i;

	new_ptr = malloc(new_size * sizeof(void *));
	if (!new_ptr)
	{
		return (0);
	}
	i = 0;
	while (i < old_size && i < new_size)
	{
		((void **)new_ptr)[i] = ((void **)(*ptr))[i];
		i++;
	}
	free(*ptr);
	*ptr = new_ptr;
	return (1);
}
