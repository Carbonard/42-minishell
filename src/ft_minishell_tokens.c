/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_tokens.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 01:06:04 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/21 01:53:59 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_input.h"

static int	is_double_metachar(char c)
{
	if (c == '<' || c == '>' || c == '|' || c == '&')
		return (1);
	return (0);
}

static int	save_token(t_context*ctx, int start, int end)
{
	char	*token;

	token = ft_substr(ctx->user_input, start, end - start);
	if (!token)
	{
		ctx->status = MS_E_MALLOC;
		return (1);
	}
	if (!token[0])
	{
		free(token);
		return (0);
	}
	if (ctx->user_input[start] != ' ' && ctx->user_input[start] != '\t')
		ft_str_lstadd_back(&ctx->input_tokens, ft_str_lstnew(token));
	return (0);
}

static int	add_token(t_context *ctx, int start, int *end)
{
	if (*end == 0)
		(*end)++;
	if (ctx->user_input[start] == ctx->user_input[*end]
		&& (is_double_metachar(ctx->user_input[start])))
		(*end)++;
	save_token(ctx, start, *end);
	start = *end;
	if (!ctx->user_input[start])
		return (start);
	(*end)++;
	if (is_metachar(ctx->user_input[start]) && !is_metachar(ctx->user_input[*end]))
	{
		if (save_token(ctx, start, *end))
			return (-1);
		start = *end;
	}
	return (start);;
}

int	get_tokens(t_context *ctx)
{
	int		i;
	int		start;
	char	quote;

	if (!ctx->user_input)
		return (1);
	i = 0;
	start = 0;
	while (ctx->user_input[i] && start >= 0)
	{
		if ((ctx->user_input[i] == '"' || ctx->user_input[i] == '\'') && !quote)
			quote = ctx->user_input[i];
		else if (quote && ctx->user_input[i] == quote)
			quote = 0;
		if (!quote && is_metachar(ctx->user_input[i]))
			start = add_token(ctx, start, &i);
		else
			i++;
	}
	if (start == -1 || add_token(ctx, start, &i) == -1)
		return (1);
	for (t_str_list *item = ctx->input_tokens; item; item = item->next)
		printf("'%s'\n", item->content);
	return (0);
}
