/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 10:50:24 by rselva-2          #+#    #+#             */
/*   Updated: 2025/10/03 08:47:48 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*new_node;

	if (lst == NULL || f == NULL || del == NULL)
		return (NULL);
	new_node = ft_lstnew(f(lst->content));
	new_list = new_node;
	while (lst->next != NULL && new_node != NULL)
	{
		lst = lst->next;
		new_node = ft_lstnew(f(lst->content));
		ft_lstadd_back(&new_list, new_node);
	}
	if (new_node == NULL)
	{
		ft_lstclear(&new_list, del);
	}
	return (new_list);
}
