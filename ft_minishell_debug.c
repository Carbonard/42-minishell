/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_debug.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 20:27:06 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/18 20:39:29 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	display_tree(t_command *tree)
{
	printf("\001\e[95m\002");
	if (!tree->cmd1)
	{
		printf("[%s]", tree->cmd);
		printf("\001\e[0m\002");
		return ;
	}
	printf("[ ");
	display_tree(tree->cmd1);
	printf(" %d ", tree->sep);
	display_tree(tree->cmd2);
	printf(" ]");
	printf("\001\e[0m\002");
}
