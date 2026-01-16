/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_main.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 14:59:51 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/16 16:41:39 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

int	main(void)
{
	int		status;
	char	*input;

	status = 1;
	while (status)
	{
		input = read_input();
		interpret_input(input);
	}
	return (0);
}