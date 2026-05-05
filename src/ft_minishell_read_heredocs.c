/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_read_heredocs.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 17:22:13 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/05 10:38:22 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_input.h"

static char	*add_line(char *here_doc, char *new_line)
{
	char	*aux;

	aux = ft_strjoin(here_doc, new_line);
	free(here_doc);
	free(new_line);
	return (aux);
}

static char	*read_hd_aux(
	t_context *ctx, char *here_doc, char *new_line, char *eof)
{
	if (new_line)
	{
		here_doc = add_line(here_doc, new_line);
		here_doc[ft_strlen(here_doc) - 1] = 0;
	}
	else if (!ctx->no_tty)
	{
		ft_putchar_fd('\n', 2);
		ft_putstr_fd(ctx->shell_name, 2);
		ft_putstr_fd(
			": warning: here-document delimited by end-of-file (wanted `", 2);
		ft_putstr_fd(eof, 2);
		ft_putstr_fd("')\n", 2);
	}
	return (here_doc);
}

static void	read_hd(t_context *ctx, char *eof)
{
	char	*here_doc;
	char	*new_line;
	int		len;

	len = ft_strlen(eof);
	here_doc = NULL;
	if (!ctx->no_tty)
		ft_putstr_fd("> ", 1);
	new_line = get_next_line(0);
	while (new_line && !g_last_signal
		&& (ft_strncmp(new_line, eof, len) || new_line[len] != '\n'))
	{
		here_doc = add_line(here_doc, new_line);
		if (!ctx->no_tty)
			ft_putstr_fd("> ", 1);
		new_line = get_next_line(0);
	}
	here_doc = read_hd_aux(ctx, here_doc, new_line, eof);
	if (here_doc)
		add_ptr(&ctx->here_docs, here_doc);
	else
		add_ptr(&ctx->here_docs, ft_calloc(1, 1));
	add_ptr(&ctx->eofs, eof);
}

void	read_heredocs(t_context *ctx)
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
			while (ctx->user_input[i + len]
				&& !is_metachar(ctx->user_input[i + len]))
				len++;
			eof = ft_substr(ctx->user_input, i, len);
			read_hd(ctx, eof);
		}
		i++;
	}
}
