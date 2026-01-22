/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_pwd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 18:27:10 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/22 17:03:15 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

int	pwd(void)
{
	char	path[MAX_PWD];

	if (!getcwd(path, MAX_PWD))
		return (MS_E_PWD_NFOUND);
	ft_putendl_fd(path, 1);
	return (MS_SUCCESS);
}
