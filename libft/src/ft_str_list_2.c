/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_list_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 12:27:38 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/28 19:02:48 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_str_lst_preinstert(t_str_list *lst, t_str_list *new)
{
	if (!lst || !new)
		return ;
	new->next = lst;
	new->prev = lst->prev;
	if (lst->prev)
		lst->prev->next = new;
	lst->prev = new;
}

t_str_list	*ft_str_lstlast(t_str_list *token)
{
	while (token && token->next)
		token = token->next;
	return (token);
}

void	ft_str_lstbreak(t_str_list *item)
{
	if (!item)
		return ;
	if (item->prev)
		item->prev->next = NULL;
	if (item->next)
		item->next->prev = NULL;
	free(item->content);
	free(item);
}
