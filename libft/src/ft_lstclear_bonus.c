/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 10:46:33 by rselva-2          #+#    #+#             */
/*   Updated: 2025/10/02 15:44:37 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*item;
	t_list	*tmp_item;

	if (lst == NULL || *lst == NULL || del == NULL)
		return ;
	item = *lst;
	while (item != NULL)
	{
		tmp_item = item;
		item = item->next;
		ft_lstdelone(tmp_item, del);
	}
	*lst = NULL;
}
