/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_parser.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyxssa <nyxssa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 16:54:26 by nyxssa            #+#    #+#             */
/*   Updated: 2026/01/17 03:52:37 by nyxssa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"


int		check_parenthesis(char *str)
{
	int	parenthesis_checker;
	int	parenthesis_checker_i;

	parenthesis_checker = 1;
	parenthesis_checker_i = 0;
	while (parenthesis_checker != 0)
		{
			parenthesis_checker_i++;
			if (str[parenthesis_checker_i] == '(')	
				parenthesis_checker++;
			else if (str[parenthesis_checker_i] == ')')	
				parenthesis_checker--;
		}
	return (parenthesis_checker_i);
}

void	remove_parenthesis(char *str)
{
	int parenthesis_checker_i;
	int	space_cnt;

	while (*str == ' ' || *str == '(')
	{
		space_cnt = 1;
		if (*str == '(')
		{
			parenthesis_checker_i = check_parenthesis(str);
			while (str[parenthesis_checker_i + space_cnt] == ' ')
				space_cnt++;
			if (str[parenthesis_checker_i + space_cnt] == 0)
			{
				str[0] = ' ';
				str[parenthesis_checker_i] = ' ';
			}
		}
		str++;
	}
}
/*int		main()
{
	char	*star;

	star = ft_strdup("(( ( (hola)(que tal)  ) ) ) ");
	remove_parenthesis(star);
	ft_printf("%s\n\n\n", star);
}*/
