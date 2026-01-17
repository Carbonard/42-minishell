/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 18:55:11 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/17 03:40:50 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	print_env(char **env)
{
	int env_i;

	env_i = 0;
	while (env[env_i])
	{
		ft_putendl_fd(env[env_i], 1);
		env_i++;
	}
}