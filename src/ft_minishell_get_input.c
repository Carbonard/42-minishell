/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_get_input.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 20:24:41 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/15 21:35:40 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_input.h"

// static void	get_prompt(t_context *ctx, char *prompt)
// {
// 	char	home[PATH_MAX];
// 	char	pwd[PATH_MAX];
// 	char	*saved_pwd;

// 	ft_strlcpy(home, find_env_value(ctx, "HOME"), 100);
// 	saved_pwd = find_env_value(ctx, "PWD");
// 	if (saved_pwd)
// 		ft_strlcpy(pwd, saved_pwd, PATH_MAX);
// 	else if (!getcwd(pwd, PATH_MAX))
// 			ft_strlcpy(pwd, ".", PATH_MAX);
// 	ft_strlcpy(prompt, "\001\e[94m\002", MAX_PROMPT);
// 	if (home[0] && !ft_strncmp(home, pwd, ft_strlen(home)))
// 	{
// 		ft_strlcat(prompt, "~", MAX_PROMPT);
// 		ft_strlcat(prompt, pwd + ft_strlen(home), MAX_PROMPT);
// 	}
// 	else
// 		ft_strlcat(prompt, pwd, MAX_PROMPT);
// 	ft_strlcat(prompt, "$ \001\e[0m\002", MAX_PROMPT);
// }

static char	*read_input_line(t_context *ctx)
{
	char				prompt[MAX_PROMPT];
	char				*read_line;
	struct sigaction	new_act;
	struct sigaction	old_act;

	ft_bzero(&new_act, sizeof(struct sigaction));
	new_act.sa_handler = rl_handler_sigint;
	sigaction(SIGINT, &new_act, &old_act);
	if (ctx->no_tty)
	{
		read_line = get_next_line(STDIN_FILENO);
		if (read_line && read_line[ft_strlen(read_line) - 1] == '\n')
			read_line[ft_strlen(read_line) - 1] = 0;
	}
	else
	{
		get_prompt(ctx, prompt);
		read_line = readline(prompt);
	}
	if (!read_line && !ctx->user_input && !ctx->no_tty)
		read_line = ft_strdup("exit");
	sigaction(SIGINT, &old_act, NULL);
	return (read_line);
}

static void	check_and_extend_input(t_context *ctx)
{
	char	*input_extension;
	char	*aux;

	ft_putstr_fd("> ", 1);
	input_extension = get_next_line(0);
	if (g_last_signal)
		return ;
	if (!input_extension && check_quotes(ctx))
		ctx->status = MS_SE_QUOTES;
	else if (!input_extension)
		ctx->status = MS_SE_EOF;
	if (!input_extension)
	{
		shell_perror(ctx, NULL);
		ft_exit(ctx, 2);
	}
	if (!g_last_signal)
	{
		aux = ft_strjoin_char(ctx->user_input, input_extension, '\n');
		free(ctx->user_input);
		free(input_extension);
		if (aux[ft_strlen(aux) - 1] == '\n')
			aux[ft_strlen(aux) - 1] = 0;
		ctx->user_input = aux;
	}
}

int	read_input(t_context *ctx)
{
	ctx->user_input = read_input_line(ctx);
	g_last_signal = 0;
	if (!ctx->user_input)
	{
		ctx->status = MS_EXIT;
		return (0);
	}
	ctx->input_lines = 1;
	if (check_syntax(ctx))
		return (0);
	while (!g_last_signal && ctx->status == MS_SUCCESS && ctx->user_input
		&& (check_quotes(ctx) || check_parenthesis(ctx) || check_operator(ctx)))
	{
		check_and_extend_input(ctx);
		ctx->input_lines++;
		check_syntax(ctx);
	}
	if (g_last_signal)
	{
		g_last_signal = 0;
		return (0);
	}
	if (ctx->status)
		return (0);
	return (1);
}
