/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_check_input.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 20:14:14 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/19 00:53:48 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_input.h"

int	check_quotes(t_context *ctx)
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

int	check_parenthesis(t_context *ctx)
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
		i += advance_quotes(ctx->user_input + i);
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

int	check_operator(t_context *ctx)
{
	int	i;

	i = 0;
	ctx->status = MS_SUCCESS;
	while (ctx->status == MS_SUCCESS && ctx->user_input[i])
	{
		if (is_operator(ctx->user_input + i))
		{
			i += is_operator(ctx->user_input + i);
			while ((ctx->user_input[i] == ' ' || ctx->user_input[i] == '\n'))
				i++;
			if (ctx->user_input[i] == 0)
				ctx->status = MS_E_SYNTAX;
		}
		else if (is_redirection(ctx->user_input + i))
		{
			i += is_redirection(ctx->user_input + i);
			while ((ctx->user_input[i] == ' '))
				i++;
			if (ctx->user_input == 0)
				ctx->status = MS_E_SYNTAX_EOF;
		}
		i++;
	}
	return (ctx->status != MS_SUCCESS);
}
