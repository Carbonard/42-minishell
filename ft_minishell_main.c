/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_main.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 14:59:51 by rselva-2          #+#    #+#             */
/*   Updated: 2026/03/31 21:32:58 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

static void	io_while(t_context *ctx)
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
		// else
		// 	print_syntax_error();
		clear_input(ctx);
	}
}

static void	check_interactive(t_context *ctx, int argc, char **argv)
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

static void	set_shell(t_context *ctx, char *shell_name)
{
	char	*shell;
	char	*var;
	char	**tmp;
	int		last_slash;

	tmp = malloc(3 * sizeof(char *));
	tmp[0] = ft_strdup("cd");
	tmp[2] = NULL;
	if (shell_name[0] != '.' && shell_name[0] != '/' && shell_name[0] != '~')
	{
		ctx->shell_name = shell_name;
		shell = find_cmd_path(ctx, shell_name);
	}
	else
	{
		shell = malloc(PATH_MAX + 20);
		last_slash = ft_strlen(shell_name);
		while (shell_name[last_slash] != '/')
			last_slash--;
		shell_name[last_slash] = 0;
		tmp[1] = ft_strdup(shell_name);
		cd(ctx, tmp);
		// ft_strlcpy(shell, "SHELL=", PATH_MAX + 20);
		getcwd(shell, PATH_MAX);
		ft_strlcat(shell, "/", PATH_MAX + 20);
		ft_strlcat(shell, shell_name + last_slash + 1, PATH_MAX + 20);
		ctx->shell_name = shell_name + last_slash + 1;
		free(tmp[1]);
		tmp[1] = ft_strdup("-");
		cd(ctx, tmp);
		free(tmp[0]);
		free(tmp[1]);
		free(tmp);
		unset(ctx, "OLDPWD");
	}
	var = ft_strjoin("SHELL=", shell);
	export(ctx, var);
	free(var);
	free(shell);
}

int	main(int argc, char **argv, char **env)
{
	t_context	ctx;
	struct sigaction	act;
	
	act.sa_handler = SIG_IGN;
	signal(SIGINT, handler_sigint);
	sigaction(SIGQUIT, &act, NULL);
	check_interactive(&ctx, argc, argv);
	save_env(&ctx, env);
	set_shell(&ctx, argv[0]);
	// ctx.cmd_tree = malloc(sizeof(t_command_tree));
	io_while(&ctx);
	free_all(&ctx);
	return (0);
}
