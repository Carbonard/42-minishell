/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_main.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 14:59:51 by rselva-2          #+#    #+#             */
/*   Updated: 2026/03/27 16:56:09 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	read_hd(t_context *ctx, char *eof)
{
	char	*here_doc;
	char	*new_line;
	char	*aux;
	int		len;

	len = ft_strlen(eof);
	here_doc = NULL;
	ft_putstr_fd("> ", 1);
	new_line = get_next_line(0);
	while (new_line
		&& (ft_strncmp(new_line, eof, len) || new_line[len] != '\n'))
	{
		aux = ft_strjoin(here_doc, new_line);
		free(here_doc);
		free(new_line);
		here_doc = aux;
		ft_putstr_fd("> ", 1);
		new_line = get_next_line(0);
	}
	if (new_line)
		free(new_line);
	else
		ft_putchar_fd('\n', 1);
	//else mensaje de error
	add_ptr(&ctx->here_docs, here_doc);
}

void	read_here_docs(t_context *ctx)
{
	int		i;
	int		len;
	char	*eof;

	i = 0;
	while (ctx->user_input[i])
	{
		if (ctx->user_input[i] == '<' && ctx->user_input[i + 1] == '<')
		{
			while (ctx->user_input[i] == '<' || ctx->user_input[i] == ' ')
				i++;
			len = 0;
			// condiciones while
			while (ctx->user_input[i + len] && ctx->user_input[i + len] != ' ')
				len++;
			eof = ft_substr(ctx->user_input, i, len);
			read_hd(ctx, eof);
			free(eof);
		}
		i++;
	}
}

int	spread_here_docs(t_command_tree *node, t_dyn_ptr *hd, int n)
{
	int	i;

	if (node->sep == NONE)
	{
		i = 0;
		while (node->cmd[i])
		{
			if (node->cmd[i] == '<' && node->cmd[i + 1] == '<')
			{
				node->here_doc = hd->arr[n];
				n++;
			}
			i++;
		}
	}
	else
	{
		n = spread_here_docs(node->cmd1, hd, n);
		n = spread_here_docs(node->cmd2, hd, n);
	}
	return (n);
}

void	io_while(t_context *ctx)
{
	ctx->status = MS_SUCCESS;
	while (ctx->status != MS_EXIT)
	{
		init_dyn_ptr(&ctx->here_docs, 1);
		if (read_input(ctx))
		{
			read_here_docs(ctx);
			ctx->cmd_tree.cmd = ctx->user_input;
			create_tree(&ctx->cmd_tree);
			spread_here_docs(&ctx->cmd_tree, &ctx->here_docs, 0);
			execute_input(ctx);
		}
		else if (ctx->status == MS_EXIT)
			return ;
		clear_input(ctx);
	}
}

void	check_interactive(t_context *ctx, int argc, char **argv)
{
	int	input_pipe[2];

	ctx->interactive = 0;
	if (!isatty(0))
		ctx->interactive = 1;
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
	if (!isatty(0))
		return ;
	pipe(input_pipe);
	dup2(input_pipe[0], STDIN_FILENO);
	close(input_pipe[0]);
	ft_putstr_fd(argv[2], input_pipe[1]);
	close(input_pipe[1]);
	ctx->interactive = 1;
}

void dn(int sig)
{
	(void) sig;
	printf("a");
}

int	main(int argc, char **argv, char **env)
{
	t_context	ctx;
	struct sigaction	act;
	
	act.sa_handler = SIG_IGN;
	signal(SIGINT, handler_sigint);
	// signal(, dn);
	// signal(SIGQUIT, do_nothing);
	sigaction(SIGQUIT, &act, NULL);
	check_interactive(&ctx, argc, argv);
	save_env(&ctx, env);
	set_shell(&ctx, argv[0]);
	// ctx.cmd_tree = malloc(sizeof(t_command_tree));
	io_while(&ctx);
	free_all(&ctx);
	return (0);
}
