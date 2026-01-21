/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dynamic_int.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 17:06:30 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/21 23:11:44 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dynarray.h"

int	init_dyn_int(t_dyn_int *d_array, size_t size)
{
	d_array->size = size;
	d_array->length = 0;
	d_array->arr = malloc(d_array->size * sizeof(int));
	if (!(d_array->arr))
	{
		d_array->size = 0;
		return (0);
	}
	return (1);
}

int	add_int(t_dyn_int *d_array, int number)
{
	size_t	new_size;

	new_size = d_array->size;
	while (d_array->length >= new_size)
		new_size *= 2;
	if (new_size != d_array->size)
	{
		if (ft_realloc((void **)&(d_array->arr),
			d_array->size * sizeof(int), new_size * sizeof(int)))
			d_array->size = new_size;
		else
			return (0);
	}
	d_array->arr[d_array->length] = number;
	d_array->length++;
	return (1);
}

void	free_dyn_int(t_dyn_int *d_array)
{
	d_array->size = 0;
	free(d_array->arr);
	d_array->arr = NULL;
}
