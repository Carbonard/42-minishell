/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 12:27:38 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/20 00:07:19 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_str_list	*ft_str_lstnew(char *content)
{
	t_str_list	*item;

	item = malloc(sizeof(t_str_list));
	if (item == NULL)
		return (NULL);
	item->content = content;
	item->next = NULL;
	return (item);
}

void	ft_str_lstadd_back(t_str_list **lst, t_str_list *new)
{
	t_str_list	*item;

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

void	ft_str_delitem(t_str_list **lst, t_str_list *item)
{
	t_str_list	*lst_aux;

	if (!lst || !(*lst) || !item)
		return ;
	lst_aux = *lst;
	if (lst_aux == item)
	{
		*lst = item->next;
		free(item->content);
		free(item);
	}
	while (lst_aux->next && lst_aux->next != item)
		lst_aux = lst_aux->next;
	if (!lst_aux->next)
		return ;
	lst_aux->next = item->next;
	free(item->content);
	free(item);
}

void	ft_str_lstclear(t_str_list **lst)
{
	t_str_list	*item;
	t_str_list	*tmp_item;

	if (lst == NULL || *lst == NULL)
		return ;
	item = *lst;
	while (item)
	{
		tmp_item = item;
		item = item->next;
		free(tmp_item->content);
		free(tmp_item);
	}
	*lst = NULL;
}
