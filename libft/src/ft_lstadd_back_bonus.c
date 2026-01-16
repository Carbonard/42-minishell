/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 10:02:48 by rselva-2          #+#    #+#             */
/*   Updated: 2025/10/02 15:58:32 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*item;

	if (lst == NULL || new == NULL)
		return ;
	item = *lst;
	if (item == NULL)
	{
		*lst = new;
		return ;
	}
	while (item->next != NULL)
	{
		item = item->next;
	}
	item->next = new;
}
