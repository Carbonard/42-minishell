/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_main.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 14:59:51 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/21 23:03:29 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

int	main(int argc, char **argv, char **env)
{
	int			status;
	t_context	ctx;

	save_env(&ctx, env);
	ctx.cmd_tree = malloc(sizeof(t_command_tree));
	status = 1;
	while (status >= 0)
	{
		read_input(&ctx);
		ctx.cmd_tree->cmd = ctx.user_input;
		create_tree(ctx.cmd_tree);
		display_tree(ctx.cmd_tree);
		putchar('\n');
		execute_input(&ctx);
	}
	(void)argc;
	(void)argv;
	return (0);
}
