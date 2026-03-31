/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_get_input.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 20:24:41 by rselva-2          #+#    #+#             */
/*   Updated: 2026/03/31 20:33:59 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	get_prompt(t_context *ctx, char *prompt)
{
	char	home[100];
	char	pwd[PATH_MAX];

	ft_strlcpy(home, find_env_value(ctx, "HOME"), 100);
	getcwd(pwd, PATH_MAX);
	ft_strlcpy(prompt, "\e[94m", MAX_PROMPT);
	if (home[0] && !ft_strncmp(home, pwd, ft_strlen(home)))
	{
		ft_strlcat(prompt, "~", MAX_PROMPT);
		ft_strlcat(prompt, pwd + ft_strlen(home), MAX_PROMPT);
	}
	else
		ft_strlcat(prompt, pwd, MAX_PROMPT);
	ft_strlcat(prompt, "$ \e[0m", MAX_PROMPT);
}

static char	*read_input_line(t_context *ctx, int print_prompt)
{
	char	prompt[MAX_PROMPT];
	char	*read_line;

	if (ctx->interactive)
	{
		read_line = get_next_line(STDIN_FILENO);
		if (read_line && read_line[ft_strlen(read_line) - 1] == '\n')
			read_line[ft_strlen(read_line) - 1] = 0;
	}
	else
	{
		if (print_prompt)
		{
			get_prompt(ctx, prompt);
			read_line = readline(prompt);
		}
		else
			read_line = readline("> ");
	}
	if (!read_line && !ctx->user_input && !ctx->interactive)
		read_line = ft_strdup("exit");
	return (read_line);
}

void	check_and_extend_input(t_context *ctx)
{
	char	*input_extension;
	char	*aux;

	input_extension = read_input_line(ctx, 0);
	if (last_signal)
	{
		free(input_extension);
		free(ctx->user_input);
		ctx->user_input = NULL;
		return ;
	}
	if (!input_extension && check_quotes(ctx))
		printf("minishell: unexpected EOF while looking for matching `\"'\n");
	else if (!input_extension)
		printf("minishell: syntax error: unexpected end of file\n");
	if (!input_extension)
		aux = ft_strdup("exit");
	else
		aux = ft_strjoin_char(ctx->user_input, input_extension, '\n');
	free(input_extension);
	free(ctx->user_input);
	ctx->user_input = aux;
}

int	read_input(t_context *ctx)
{
	ctx->status = MS_SUCCESS;
	ctx->user_input = read_input_line(ctx, 1);
	last_signal = 0;
	if (!ctx->user_input)
	{
		ctx->status = MS_EXIT;
		return (0);
	}
	if (check_syntax(ctx->user_input))
		return (0);
	while (!last_signal && !check_syntax(ctx->user_input)
		&& (check_quotes(ctx) || check_parenthesis(ctx) || check_operator(ctx)))
		check_and_extend_input(ctx);
	if (last_signal)
	{
		printf("signal: %d\n", last_signal);
		last_signal = 0;
		return (0);
	}
	if (check_syntax(ctx->user_input))
		return (0);
	add_history(ctx->user_input);
	// printf("final input: //%s//\n", ctx->user_input);
	return (1);
}
