/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_debug.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyxssa <nyxssa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 20:27:06 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/19 22:23:41 by nyxssa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	display_tree(t_command *tree)
{
	char	*separators[] = {"", "&&", "||", "|"};
	if (!tree->cmd1)
	{
		printf("\001\e[95m\002%s\001\e[0m\002", tree->cmd);
		return ;
	}
	printf("\001\e[95m\002[ ");
	display_tree(tree->cmd1);
	printf("\001\e[95m\002 %s ", separators[tree->sep]);
	display_tree(tree->cmd2);
	printf("\001\e[95m\002 ]\001\e[0m\002");
}
