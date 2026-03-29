/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_get_input.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 20:24:41 by rselva-2          #+#    #+#             */
/*   Updated: 2026/03/29 19:33:04 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	get_prompt(t_context *ctx, char *prompt)
{
	char	home[100];
	char	pwd[MAX_PWD];

	ft_strlcpy(home, find_env_value(ctx, "HOME"), 100);
	getcwd(pwd, MAX_PWD);
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

static int	check_quotes(t_context *ctx)
{
	int		i;
	char	quote;


	i = 0;
	while (ctx->user_input[i])
	{
		if (ctx->user_input[i] == '\'' || ctx->user_input[i] == '"')
		{
			quote = ctx->user_input[i];
			i++;
			while (ctx->user_input[i] && ctx->user_input[i] != quote)
				i++;
			if (!ctx->user_input[i])
			{
				ctx->status = MS_OPENQUOTE;
				return (1);
			}
		}
		i++;
	}
	return (0);
}

static int	check_parenthesis(t_context *ctx)
{
	int	i;
	int	parenthesis_checker;

	i = 0;
	parenthesis_checker = 0;
	while (ctx->user_input[i])
	{
		if (ctx->user_input[i] == '(')
			parenthesis_checker++;
		if (ctx->user_input[i] == ')')
			parenthesis_checker--;
		if (parenthesis_checker < 0)
		{
			ctx->status = MS_E_SYNTAX;
			return (1);
		}
		i++;
	}
	if (parenthesis_checker != 0)
		return (1);
	return (0);
}

static int	is_operator(char *str)
{
	if ((str[0] == '&' && str[1] == '&') || (str[0] == '|' && str[1] == '|'))
		return (2);
	if (str[0] == '|')
		return (1);
	return (0);
}

int	is_redirection(char *str)
{
	if ((str[0] == '<' && str[1] == '<') || (str[0] == '>' && str[1] == '>'))
		return (2);
	if (str[0] == '<' || str[0] == '>')
		return (1);
	return (0);
}

static int	check_operator(t_context *ctx)
{
	int	i;

	i = 0;
	while (ctx->user_input[i])
	{
		if (is_operator(ctx->user_input + i))
		{
// printf("1.%s\n", ctx->user_input + i);
			i += is_operator(ctx->user_input + i);
// printf("2.%s\n", ctx->user_input + i);
			while ((ctx->user_input[i] == ' ' || ctx->user_input[i] == '\n'))
				i++;
// printf("3.%s\n", ctx->user_input + i);
			if (ctx->user_input[i] == 0)
			{
				ctx->status = MS_E_SYNTAX;
				return (1);
			}
		}
		else if (is_redirection(ctx->user_input + i))
		{
			i += is_redirection(ctx->user_input + i);
			while ((ctx->user_input[i] == ' '))
				i++;
			if (ctx->user_input == 0)
			{
				ctx->status = MS_E_SYNTAX_EOF;
				return (1);
			}
		}
		i++;
	}
	return (0);
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

int	read_input(t_context *ctx)
{
	char	*input_extension;
	char	*aux;

	ctx->status = MS_SUCCESS;
	ctx->user_input = read_input_line(ctx, 1);
	if (!ctx->user_input)
	{
		ctx->status = MS_EXIT;
		return (0);
	}
	if (check_input(ctx->user_input))
		return (0);
	while ((check_quotes(ctx) || check_parenthesis(ctx) || check_operator(ctx)))
	{
		input_extension = read_input_line(ctx, 0);
		aux = ft_strjoin_char(ctx->user_input, input_extension, '\n');
		free(input_extension);
		free(ctx->user_input);
		ctx->user_input = aux;
	}
	add_history(ctx->user_input);
	return (1);
}
