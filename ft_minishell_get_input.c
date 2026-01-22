/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_get_input.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 20:24:41 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/22 18:29:45 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

static int	get_prompt(char *prompt)
{
	char	home[100];
	int		length;

	getcwd(prompt, MAX_PWD);
	length = ft_strlen(prompt);
	prompt[length] = '$';
	prompt[length + 1] = ' ';
	prompt[length + 2] = 0;
	ft_strlcpy(home, "/home/", 100);
	ft_strlcat(home, getenv("USER"), 100);
	if (!ft_strncmp(prompt, home, ft_strlen(home)))
	{
		prompt[ft_strlen(home) - 1] = '~';
		return (ft_strlen(home) - 1);
	}
	else
		return (0);
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

static int	is_redirection(char *str)
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

void	read_input(t_context *ctx)
{
	char	prompt[MAX_PWD + 3];
	char	*input_extension;
	char	*aux;
	int		start;

	ctx->status = MS_SUCCESS;
	start = get_prompt(prompt);
	if (ctx->interactive)
		ctx->user_input = get_next_line(STDIN_FILENO);
	else
		ctx->user_input = readline(prompt + start);
	if (!ctx->user_input)
	{
		ctx->user_input = ft_strdup("exit");
		printf("exit\n");
		return ;
	}
	while ((check_quotes(ctx) || check_parenthesis(ctx) || check_operator(ctx)))
	{
		if (ctx->interactive)
			input_extension = get_next_line(STDIN_FILENO);
		else
			input_extension = readline("> ");
		aux = ft_strjoin_char(ctx->user_input, input_extension, '\n');
		free(ctx->user_input);
		ctx->user_input = aux;
	}
}
