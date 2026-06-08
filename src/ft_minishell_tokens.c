/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_tokens.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 01:06:04 by rselva-2          #+#    #+#             */
/*   Updated: 2026/06/07 20:06:55 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_input.h"

static int	is_double_metachar(char c)
{
	if (c == '<' || c == '>' || c == '|' || c == '&')
		return (1);
	return (0);
}

static int	save_token(t_str_list **tokens, char *input, int start, int end)
{
	char		*token;
	t_str_list	*token_list_node;

	token = ft_substr(input, start, end - start);
	if (!token)
		return (1);
	if (!token[0])
	{
		free(token);
		return (0);
	}
	if (!ft_strchr(SPACES, input[start]))
	{
		token_list_node = ft_str_lstnew(token);
		if (!token_list_node)
		{
			free(token);
			return (1);
		}
		ft_str_lstadd_back(tokens, token_list_node);
	}
	else
		free(token);
	return (0);
}

static int	add_token(t_str_list **tokens, char *input, int start, int *end)
{
	if (*end == 0)
		(*end)++;
	if (input[start] == input[*end]
		&& (is_double_metachar(input[start])))
		(*end)++;
	if (save_token(tokens, input, start, *end))
		return (-1);
	start = *end;
	if (!input[start])
		return (start);
	(*end)++;
	if (ft_strchr(METACHARACTERS, input[start])
		&& !ft_strchr(METACHARACTERS, input[*end]))
	{
		if (save_token(tokens, input, start, *end))
			return (-1);
		start = *end;
	}
	return (start);
}

static void	get_tokens_loop(t_context *ctx, char *input, char *delimiters,
				t_str_list **tokens)
{
	int			i;
	int			start;
	char		quote;

	i = 0;
	start = 0;
	quote = 0;
	*tokens = NULL;
	while (input[i] && start >= 0)
	{
		if ((input[i] == '"' || input[i] == '\'') && !quote)
			quote = input[i];
		else if (quote && input[i] == quote)
			quote = 0;
		if (!quote && ft_strchr(delimiters, input[i]))
			start = add_token(tokens, input, start, &i);
		else
			i++;
	}
	if (start == -1 || add_token(tokens, input, start, &i) == -1)
	{
		ft_str_lstclear(tokens, free);
		malloc_exit(ctx);
	}
}

t_str_list	*get_tokens(t_context *ctx, char *input, char *delimiters)
{
	t_str_list	*tokens;

	if (!input || !input[0])
		return (NULL);
	get_tokens_loop(ctx, input, delimiters, &tokens);
	return (tokens);
}
