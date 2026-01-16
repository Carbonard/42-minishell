/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 23:34:37 by rselva-2          #+#    #+#             */
/*   Updated: 2025/12/04 21:37:08 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

static int	add_text(char **line, int fd, int *read_bytes)
{
	char	buffer[BUFFER_SIZE + 1];
	char	*tmp;

	*read_bytes = read(fd, buffer, BUFFER_SIZE);
	if (*read_bytes == 0)
		return (0);
	if (*read_bytes < 0)
	{
		free(*line);
		*line = NULL;
		return (0);
	}
	buffer[*read_bytes] = 0;
	tmp = ft_strjoin(*line, buffer);
	free(*line);
	*line = tmp;
	if (!tmp)
		return (0);
	return (1);
}

char	*get_next_line(int fd)
{
	static char	saved[1024][BUFFER_SIZE];
	char	*line;
	char	*endl;
	int		read_bytes;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	if (saved[fd][0])
		line = ft_strdup(saved[fd]);
	else
		line = NULL;
	read_bytes = 1;
	endl = ft_strchr(line, '\n');
	while (!endl && add_text(&line, fd, &read_bytes))
		endl = ft_strchr(line, '\n');
	if (endl)
	{
		ft_strlcpy(saved[fd], endl + 1, BUFFER_SIZE);
		endl[1] = 0;
	}
	else if (line)
	{
		saved[fd][0] = 0;
	}
	return (line);	
}
