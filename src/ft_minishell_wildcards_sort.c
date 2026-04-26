/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_wildcards_sort.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 15:51:23 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/26 12:07:25 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_execution.h"

static int	lowalnumcmp(char *s1, char *s2)
{
	int	i1;
	int	i2;

	i1 = 0;
	i2 = 0;
	while (s1[i1] && s2[i2])
	{
		while (!ft_isalnum(s1[i1]) && s1[i1])
			i1++;
		while (!ft_isalnum(s2[i2]) && s2[i2])
			i2++;
		if ((ft_tolower(s2[i2]) - ft_tolower(s1[i1])) > 0)
			return (-1);
		if ((ft_tolower(s2[i2]) - ft_tolower(s1[i1])) < 0)
			return (1);
		if (s1[i1])
			i1++;
		if (s2[i2])
			i2++;
	}
	return (0);
}

static int	alphacmp(char *s1, char *s2)
{
	int	i1;
	int	i2;

	i1 = 0;
	i2 = 0;
	while (s1[i1] && s2[i2])
	{
		while (!ft_isalpha(s1[i1]) && s1[i1])
			i1++;
		while (!ft_isalpha(s2[i2]) && s2[i2])
			i2++;
		if ((s2[i2] - s1[i1]) > 0)
			return (-1);
		if ((s2[i2] - s1[i1]) < 0)
			return (1);
		if (s1[i1])
			i1++;
		if (s2[i2])
			i2++;
	}
	return (0);
}

static int	direntry_cmp(char *s1, char *s2)
{
	int	cmp;

	cmp = lowalnumcmp(s1, s2);
	if (cmp)
		return (cmp);
	cmp = alphacmp(s1, s2);
	if (cmp)
		return (cmp);
	return (ft_strncmp(s1, s2, ft_strlen(s1)));
}

void	sort_entries(char **arr)
{
	int		main_i;
	int		secondary_i;
	char	*tmp;

	main_i = 0;
	while (arr[main_i])
	{
		secondary_i = main_i + 1;
		while (arr[secondary_i])
		{
			if (direntry_cmp(arr[main_i], arr[secondary_i]) > 0)
			{
				tmp = arr[main_i];
				arr[main_i] = arr[secondary_i];
				arr[secondary_i] = tmp;
			}
			secondary_i++;
		}
		main_i++;
	}
	return ;
}
