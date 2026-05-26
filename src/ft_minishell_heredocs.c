/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_heredocs.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 11:46:47 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/25 20:08:03 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_input.h"
#include "ft_minishell_execution.h"

void	expand_heredoc(t_context *ctx)
{
	size_t	hd_i;

	hd_i = 0;
	while (hd_i < ctx->here_docs.length)
	{
		if (ctx->here_docs.arr[hd_i])
			ctx->here_docs.arr[hd_i]
				= expand_input(ctx, ctx->here_docs.arr[hd_i]);
		hd_i++;
	}
}

static int	assign_heredoc(
	t_command_tree *node, t_dyn_ptr *hd, t_dyn_ptr *eofs, int n)
{
	t_str_list	*token;
	char		*last_line;

	token = node->cmd_tokens;
	while (token)
	{
		if (token->content[0] == '<' && token->content[1] == '<')
		{
			if (hd->arr[n])
			{
				last_line = ft_strrchr(hd->arr[n], '\n');
				if (!last_line && !ft_strncmp(hd->arr[n],
						eofs->arr[n], ft_strlen(eofs->arr[n])))
					hd->arr[n][0] = 0;
				else if (last_line && (!ft_strncmp(last_line + 1, eofs->arr[n],
							ft_strlen(eofs->arr[n]) + 1)))
					*(last_line + 1) = 0;
			}
			node->here_doc = hd->arr[n];
			n++;
		}
		token = token->next;
	}
	return (n);
}

int	spread_heredocs(t_command_tree *node, t_dyn_ptr *hd, t_dyn_ptr *eofs, int n)
{
	if (node->sep == NONE)
	{
		n = assign_heredoc(node, hd, eofs, n);
	}
	else
	{
		n = spread_heredocs(node->cmd1, hd, eofs, n);
		n = spread_heredocs(node->cmd2, hd, eofs, n);
	}
	return (n);
}
