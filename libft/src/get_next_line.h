/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 23:37:03 by rselva-2          #+#    #+#             */
/*   Updated: 2025/12/04 21:38:20 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include "libft.h"
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 9
# endif

typedef struct s_fdlist
{
	int				fd;
	char			*text;
	struct s_fdlist	*next;
}	t_fdlist;

char	*get_next_line(int fd);

#endif