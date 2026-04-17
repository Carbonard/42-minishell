/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_main.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 14:59:51 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/17 21:56:37 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_builtins.h"
#include "ft_minishell_input.h"
#include "ft_minishell_execution.h"

int	g_last_signal;

void	rl_handler_sigint(int sig)
{
	(void)sig;
	g_last_signal = sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	return ;
}

void	generic_handler_sigint(int sig)
{
	(void)sig;
	g_last_signal = sig;
	write(1, "\n", 1);
	// rl_on_new_line();
	// rl_replace_line("", 0);
	// rl_clear_history();
	return ;
}

static void	remove_newlines(char *input)
{
	int 	i;
	char	quote;

	if (!input || !input[0])
		return ;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			quote = input[i];
			i++;
			while (input[i] && input[i] != quote)
				i++;
		}
		if (input[i] == '\n' && input[i + 1])
			input[i] = ' ';
		i++;
	}
	if (input[i-1] == '\n')
		input[i-1] = 0;
}

void add_input_history(t_context *ctx)
{
	char	*history_entry;
	char	*aux;
	size_t	i;

	remove_newlines(ctx->user_input);
	history_entry = ft_strdup(ctx->user_input);
	i = 0;
	while (i < ctx->here_docs.length)
	{
		aux = ft_strjoin_char(history_entry, ctx->here_docs.arr[i], '\n');
		free(history_entry);
		history_entry = aux;
		i++;
	}
	if (history_entry && history_entry[0] && history_entry[0]!= ' ')
		add_history(history_entry);
	free(history_entry);
}

static void	io_while(t_context *ctx);

void manage_multiple_input(t_context *ctx)
{
	int	pipe_fds[2];
	int	original_in;

	original_in = dup(STDIN_FILENO);
	pipe(pipe_fds);
	dup2(pipe_fds[0], STDIN_FILENO);
	ft_putstr_fd(ctx->user_input, pipe_fds[1]);
	close(pipe_fds[0]);
	close(pipe_fds[1]);
	clear_input(ctx);
	ctx->interactive = 1;
	io_while(ctx);
	ctx->interactive = 0;
	dup2(original_in, STDIN_FILENO);
	close(original_in);
}

static void	io_while(t_context *ctx)
{
	while (ctx->status != MS_EXIT)
	{
		ctx->status = MS_SUCCESS;
		init_dyn_ptr(&ctx->here_docs, 0);
		init_dyn_ptr(&ctx->eofs, 0);
		if (read_input(ctx))
		{
			if (ft_strchr(ctx->user_input, '\n'))
				manage_multiple_input(ctx);
			else
			{
				read_here_docs(ctx);
				add_input_history(ctx);
				if (g_last_signal)
				{
					clear_input(ctx);
					continue;
				}
				expand_heredoc(ctx);
				ctx->cmd_tree.cmd = ctx->user_input;
				create_tree(&ctx->cmd_tree);
				spread_here_docs(&ctx->cmd_tree, &ctx->here_docs, &ctx->eofs, 0);
				execute_input(ctx);
			}
		}
		else if (ctx->status == MS_EXIT)
			break ;
		clear_input(ctx);
	}
	ctx->status = MS_SUCCESS;
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
	t_context			ctx;
	struct sigaction	act, actt;

	g_last_signal = 0;
	ctx = (t_context){.cmd_tree.cmd1 = NULL, .cmd_tree.cmd2 = NULL,
		.status = MS_SUCCESS, .exit_status = 0, .user_input = NULL};
	actt = (struct sigaction){.sa_handler = generic_handler_sigint};
	act = (struct sigaction){.sa_handler = SIG_IGN};
	sigaction(SIGINT, &actt, NULL);
	act.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &act, NULL);
	save_env(&ctx, env);
	set_shell(&ctx, argv[0]);
	check_interactive(&ctx, argc, argv);
	io_while(&ctx);
	free_all(&ctx);
	return (ctx.exit_status);
}
