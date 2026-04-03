/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_main.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 14:59:51 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/03 15:46:08 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

static void	io_while(t_context *ctx)
{
	while (ctx->status != MS_EXIT)
	{
		ctx->status = MS_SUCCESS;
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
		custom_error(ctx->shell_name, "-c: option requires an argument");
		silent_exit(ctx, 2);
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
	char	shell[PATH_MAX];
	char	*var;
	char	*cd_argv[3];
	int		last_slash;

	if (shell_name[0] != '.' && shell_name[0] != '/' && shell_name[0] != '~')
	{
		ctx->shell_name = shell_name;
		find_cmd_path(ctx, shell, shell_name);
	}
	else
	{
		last_slash = ft_strlen(shell_name);
		while (shell_name[last_slash] != '/')
			last_slash--;
		shell_name[last_slash] = 0;
		cd_argv[0] = "cd";
		cd_argv[1] = shell_name;
		cd_argv[2] = NULL;
		cd(ctx, cd_argv);
		getcwd(shell, PATH_MAX);
		ft_strlcat(shell, "/", PATH_MAX);
		ft_strlcat(shell, shell_name + last_slash + 1, PATH_MAX);
		ctx->shell_name = shell_name + last_slash + 1;
		cd_argv[1] = ft_strdup("-");
		cd(ctx, cd_argv);
		free(cd_argv[1]);
		unset(ctx, "OLDPWD");
	}
	var = ft_strjoin("SHELL=", shell);
	export(ctx, var);
	free(var);
}

int	main(int argc, char **argv, char **env)
{
	t_context	ctx;
	struct sigaction	act;

	ctx.cmd_tree.cmd1 = NULL;
	ctx.cmd_tree.cmd2 = NULL;
	ctx.status = MS_SUCCESS;
	ctx.exit_status = 0;
	act.sa_handler = SIG_IGN;
	signal(SIGINT, handler_sigint);
	sigaction(SIGQUIT, &act, NULL);
	save_env(&ctx, env);
	set_shell(&ctx, argv[0]);
	check_interactive(&ctx, argc, argv);
	io_while(&ctx);
	free_all(&ctx);
	return (0);
}
