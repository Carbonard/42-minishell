/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_execute_tree.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 19:24:30 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/18 20:10:08 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

int	execute(t_command *command, char **env)
{
	char			**split;
	t_redirection	redir;
	int				status;

	if (!command->sep)
	{
		split = split_cmd(command->cmd, &redir);
		status = check_build_ins(split, env);
		if (status >= 0)
			return (0);
		printf("Lo va a hacer tu madre jaja saludos\n");
	}
	return (0);
}
