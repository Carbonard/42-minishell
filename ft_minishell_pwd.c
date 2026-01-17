/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_pwd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 18:27:10 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/17 02:40:07 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	pwd(void)
{
	char	path[MAX_PWD];

	getcwd(path, MAX_PWD);
	ft_putendl_fd(path, 1);
}
