/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_expand_variables.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 23:34:26 by rselva-2          #+#    #+#             */
/*   Updated: 2026/06/06 22:11:39 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_execution.h"

static int	inside_dq(char *str, int i)
{
	int	inside_double_quotes;
	int	j;

	inside_double_quotes = 0;
	j = 0;
	while (j < i)
	{
		if (str[j] == '"')
			inside_double_quotes = !inside_double_quotes;
		j++;
	}
	return (inside_double_quotes);
}

static int	need_to_expand(char *str, int i)
{
	int	inside_double_quotes;

	inside_double_quotes = inside_dq(str, i);
	while (str[i])
	{
		if (!inside_double_quotes && str[i] == '\'')
		{
			i++;
			while (str[i] != '\'')
				i++;
		}
		else if (str[i] == '"')
			inside_double_quotes = !inside_double_quotes;
		else if (str[i] == '$'
			&& (ft_isalpha(str[i + 1])
				|| str[i + 1] == '_'
				|| str[i + 1] == '?'))
			return (i);
		i++;
	}
	return (-1);
}

char	*expand_variables(t_context *ctx, char *input)
{
	int		i;

	i = need_to_expand(input, 0);
	while (i >= 0 && input)
	{
		input = expand_variable(ctx, input, &i);
		if (input)
			i = need_to_expand(input, i);
	}
	return (input);
}
