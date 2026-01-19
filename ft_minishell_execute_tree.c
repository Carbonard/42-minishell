/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_execute_tree.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyxssa <nyxssa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 19:24:30 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/19 22:15:27 by nyxssa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

int	execute(t_command *command, char **env)
{
	char			**split;
	t_redirection	redir;
	int				status;
printf("\e[32m%s\nsep: %i\e[0m\n", command->cmd, command->sep);
	if (!command->sep)
	{
		split = split_cmd(command->cmd, &redir);
		status = check_build_ins(split, env);
		if (status >= 0)
			return (0);
		printf("Lo va a hacer tu madre jaja saludos\n");
	}
	else
	{
		if (command->cmd1)
			execute(command->cmd1, env);
		if (command->cmd2)
			execute(command->cmd2, env);
	}
	return (0);
}
