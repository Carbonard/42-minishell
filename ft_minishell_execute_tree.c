/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_execute_tree.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 19:24:30 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/20 00:00:33 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

int	execute(t_context *ctx, t_command *command)
{
	char			**split;
	t_redirection	redir;
	int				status;
// printf("\e[32m%s\nsep: %i\e[0m\n", command->cmd, command->sep);
	if (!command->sep)
	{
		split = split_cmd(command->cmd, &redir);
		status = check_build_ins(ctx, split);
		if (status == MS_BUILT_IN_EXECUTED)
			return (0);
		printf("Lo va a hacer tu madre jaja saludos\n");
	}
	else
	{
		if (command->cmd1)
			execute(ctx, command->cmd1);
		if (command->cmd2)
			execute(ctx, command->cmd2);
	}
	return (0);
}
