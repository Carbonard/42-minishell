/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_tokens.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 01:06:04 by rselva-2          #+#    #+#             */
/*   Updated: 2026/06/02 10:21:58 by rselva-2         ###   ########.fr       */
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
	char	*token;

	token = ft_substr(input, start, end - start);
	if (!token)
		return (1);
	if (!token[0])
	{
		free(token);
		return (0);
	}
	if (!ft_strchr(SPACES, input[start]))
		ft_str_lstadd_back(tokens, ft_str_lstnew(token));
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
	save_token(tokens, input, start, *end);
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

t_str_list	*get_tokens(char *input, char *delimiters)
{
	t_str_list	*tokens;
	int			i;
	int			start;
	char		quote;

	if (!input || !input[0])
		return (NULL);
	i = 0;
	start = 0;
	quote = 0;
	tokens = NULL;
	while (input[i] && start >= 0)
	{
		if ((input[i] == '"' || input[i] == '\'') && !quote)
			quote = input[i];
		else if (quote && input[i] == quote)
			quote = 0;
		if (!quote && ft_strchr(delimiters, input[i]))
			start = add_token(&tokens, input, start, &i);
		else
			i++;
	}
	if (start == -1 || add_token(&tokens, input, start, &i) == -1)
		return (ft_str_lstclear(&tokens, free));
	return (tokens);
}
