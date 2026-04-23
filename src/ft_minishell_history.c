/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_history.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elangari <elangari@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 16:02:40 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/19 19:49:22 by elangari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

static void	remove_newlines(char *input)
{
	int		i;
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
	if (input[i - 1] == '\n')
		input[i - 1] = 0;
}

void	add_input_history(t_context *ctx)
{
	char	*history_entry;
	char	*aux;
	size_t	i;

	if (ctx->no_tty)
		return ;
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
	if (history_entry && history_entry[0] && history_entry[0] != ' ')
		add_history(history_entry);
	free(history_entry);
}
