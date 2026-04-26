/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dynarray.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 01:14:42 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/26 12:52:21 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
// #include <stdint.h>

typedef struct s_dyn_int
{
	int		*arr;
	size_t	size;
	size_t	length;
}	t_dyn_int;

// typedef struct s_dyn_char
// {
// 	char	*arr;
// 	size_t	size;
// }	t_dyn_char;

typedef struct s_dyn_ptr
{
	char	**arr;
	size_t	size;
	size_t	length;
}	t_dyn_ptr;

// realloc
int		ft_realloc(void **ptr, size_t old_size, size_t new_size);
int		ft_realloc2(void ***ptr, size_t old_size, size_t new_size);
// Int
int		add_int(t_dyn_int *d_array, int number);
int		init_dyn_int(t_dyn_int *d_array, size_t size);
void	free_dyn_int(t_dyn_int *d_array);
// Ptr
int		init_dyn_ptr(t_dyn_ptr *d_array, size_t size);
int		add_ptr(t_dyn_ptr *d_array, char *str);
void	free_dyn_ptr(t_dyn_ptr *d_array);
