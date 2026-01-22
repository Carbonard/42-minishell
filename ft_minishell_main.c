/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_main.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 14:59:51 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/22 18:31:11 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	io_while(t_context *ctx)
{
	while (ctx->status != MS_EXIT)
	{
		read_input(ctx);
		ctx->cmd_tree->cmd = ctx->user_input;
		create_tree(ctx->cmd_tree);
		// display_tree(ctx->cmd_tree);
		// putchar('\n');
		execute_input(ctx);
	}
}
void	check_interactive(t_context *ctx, int argc, char **argv)
{
	int	input_pipe[2];

	ctx->interactive = 0;
	if (argc < 2)
		return ;
	if (!valid_flag(argv[1], 'c'))
	{
		//read_from_file(?);
		return ;
	}
	if (argc < 3)
	{
		printf("minishell: -c: option requires an argument\n");
		exit (2);
	}
	pipe(input_pipe);
	dup2(input_pipe[0], STDIN_FILENO);
	close(input_pipe[0]);
	ft_putstr_fd(argv[2], input_pipe[1]);
	close(input_pipe[1]);
	ctx->interactive = 1;
}

int	main(int argc, char **argv, char **env)
{
	t_context	ctx;

	check_interactive(&ctx, argc, argv);
	save_env(&ctx, env);
	ctx.cmd_tree = malloc(sizeof(t_command_tree));
	io_while(&ctx);
	return (0);
}
