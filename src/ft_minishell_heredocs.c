/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_heredocs.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 17:22:13 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/17 22:06:17 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_input.h"
#include "ft_minishell_execution.h"

void	read_hd(t_context *ctx, char *eof)
{
	char	*here_doc;
	char	*new_line;
	char	*aux;
	int		len;

	len = ft_strlen(eof);
	here_doc = NULL;
	if (!ctx->interactive)
		ft_putstr_fd("> ", 1);
	new_line = get_next_line(0);
	while (new_line && !g_last_signal
		&& (ft_strncmp(new_line, eof, len) || new_line[len] != '\n'))
	{
		aux = ft_strjoin(here_doc, new_line);
		free(here_doc);
		free(new_line);
		here_doc = aux;
		if (!ctx->interactive)
			ft_putstr_fd("> ", 1);
		new_line = get_next_line(0);
	}
	if (new_line)
	{
		aux = ft_strjoin(here_doc, new_line);
		free(here_doc);
		free(new_line);
		here_doc = aux;
		here_doc[ft_strlen(here_doc) - 1] = 0;
		// free(new_line);
	}
	//else mensaje de error
	else if (!ctx->interactive)
		ft_putchar_fd('\n', 1);
	add_ptr(&ctx->here_docs, here_doc);
	add_ptr(&ctx->eofs, eof);
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
			// free(eof);
		}
		i++;
	}
}

void	expand_heredoc(t_context *ctx)
{
	size_t	hd_i;
	// int		str_i;

	hd_i = 0;
	while (hd_i < ctx->here_docs.length)
	{
		if (ctx->here_docs.arr[hd_i])
			ctx->here_docs.arr[hd_i] = expand_input(ctx, ctx->here_docs.arr[hd_i]);
		// str_i = need_to_expand(ctx->here_docs.arr[hd_i], 0);
		// while (str_i > 0)
		// {
		// 	ctx->here_docs.arr[hd_i] = expand_cmd(ctx, ctx->here_docs.arr[hd_i], &str_i);
		// 	str_i++;
		// 	str_i = need_to_expand(ctx->here_docs.arr[hd_i], str_i);
		// }
		hd_i++;
	}
}

int	spread_here_docs(t_command_tree *node, t_dyn_ptr *hd, t_dyn_ptr *eofs, int n)
{
	int		i;
	char	*last_line;

	if (node->sep == NONE)
	{
		i = 0;
		while (node->cmd[i])
		{
			if (node->cmd[i] == '<' && node->cmd[i + 1] == '<')
			{
				last_line = ft_strrchr(hd->arr[n], '\n');
				if (!last_line && !ft_strncmp(hd->arr[n], eofs->arr[n], ft_strlen(eofs->arr[n])))
					hd->arr[n][0] = 0;
				else if (last_line && (!ft_strncmp(last_line + 1, eofs->arr[n], ft_strlen(eofs->arr[n]) + 1)))
					*(last_line + 1) = 0;
				node->here_doc = hd->arr[n];
				n++;
			}
			i++;
		}
	}
	else
	{
		n = spread_here_docs(node->cmd1, hd, eofs, n);
		n = spread_here_docs(node->cmd2, hd, eofs, n);
	}
	return (n);
}
