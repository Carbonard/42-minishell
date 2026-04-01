/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_heredocs.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 17:22:13 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/01 15:01:03 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	read_hd(t_context *ctx, char *eof)
{
	char	*here_doc;
	char	*new_line;
	char	*aux;
	int		len;

	len = ft_strlen(eof);
	here_doc = NULL;
	ft_putstr_fd("> ", 1);
	new_line = get_next_line(0);
	while (new_line
		&& (ft_strncmp(new_line, eof, len) || new_line[len] != '\n'))
	{
		aux = ft_strjoin(here_doc, new_line);
		free(here_doc);
		free(new_line);
		here_doc = aux;
		ft_putstr_fd("> ", 1);
		new_line = get_next_line(0);
	}
	if (new_line)
		free(new_line);
	else
		ft_putchar_fd('\n', 1);
	//else mensaje de error
	add_ptr(&ctx->here_docs, here_doc);
}

int	is_metachar(char c)
{
	if (c == '<' || c == '>' || c == '|' || c == '&' || c == ' ' || c == '\t'
		|| c == '\n' || c == '(' || c == ')' || c == ';')
		return (1);
	return (0);
}

void	read_here_docs(t_context *ctx)
{
	int		i;
	int		len;
	char	*eof;

	i = 0;
	while (ctx->user_input[i])
	{
		if (ctx->user_input[i] == '<' && ctx->user_input[i + 1] == '<')
		{
			while (ctx->user_input[i] == '<' || ctx->user_input[i] == ' ')
				i++;
			len = 0;
			// condiciones while
			while (ctx->user_input[i + len]
				&& !is_metachar(ctx->user_input[i + len]))
				len++;
			eof = ft_substr(ctx->user_input, i, len);
			read_hd(ctx, eof);
			free(eof);
		}
		i++;
	}
}

int	spread_here_docs(t_command_tree *node, t_dyn_ptr *hd, int n)
{
	int	i;

	if (node->sep == NONE)
	{
		i = 0;
		while (node->cmd[i])
		{
			if (node->cmd[i] == '<' && node->cmd[i + 1] == '<')
			{
				node->here_doc = hd->arr[n];
				n++;
			}
			i++;
		}
	}
	else
	{
		n = spread_here_docs(node->cmd1, hd, n);
		n = spread_here_docs(node->cmd2, hd, n);
	}
	return (n);
}
