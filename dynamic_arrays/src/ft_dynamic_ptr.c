/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dynamic_ptr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 17:06:30 by rselva-2          #+#    #+#             */
/*   Updated: 2026/06/21 17:11:38 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dynarray.h"

int	init_dyn_ptr(t_dyn_ptr *d_array, size_t size)
{
	d_array->size = size;
	d_array->length = 0;
	if (size == 0)
	{
		d_array->arr = NULL;
		return (1);
	}
	d_array->arr = malloc(d_array->size * sizeof(char *));
	if (!(d_array->arr))
	{
		d_array->size = 0;
		return (0);
	}
	if (size)
		d_array->arr[0] = NULL;
	return (1);
}

int	add_ptr(t_dyn_ptr *d_array, char *str)
{
	if (d_array->size == 0)
	{
		d_array->size = 2;
		d_array->arr = malloc(2 * sizeof(char *));
	}
	if (d_array->length + 1 >= d_array->size)
	{
		if (ft_realloc2((void ***)&(d_array->arr),
				d_array->size, d_array->size * 2))
			d_array->size *= 2;
		else
			return (0);
	}
	d_array->arr[d_array->length] = str;
	d_array->length++;
	d_array->arr[d_array->length] = NULL;
	return (1);
}

void	free_dyn_ptr(t_dyn_ptr *d_array)
{
	size_t	i;

	i = 0;
	while (i < d_array->length)
	{
		free(d_array->arr[i]);
		i++;
	}
	free(d_array->arr);
	d_array->length = 0;
	d_array->size = 0;
	d_array->arr = NULL;
}
