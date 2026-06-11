/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_read_heredocs.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 17:22:13 by rselva-2          #+#    #+#             */
/*   Updated: 2026/06/12 01:09:41 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_input.h"
#include "ft_minishell_builtins.h"
#include "ft_minishell_execution.h"

static char	*read_line(t_context *ctx)
{
	char	*line;

	if (!ctx->no_tty)
		ft_putstr_fd("> ", 1);
	line = get_next_line_force_nl(0);
	if (line && line [0] && line[ft_strlen(line) - 1] == '\n')
		line[ft_strlen(line) - 1] = 0;
	return (line);
}

static void	heredoc_ending_error(char *shell_name, char *eof)
{
	ft_putchar_fd('\n', 2);
	ft_putstr_fd(shell_name, 2);
	ft_putstr_fd(
		": warning: here-document delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(eof, 2);
	ft_putstr_fd("')\n", 2);
}

static void	read_hd(t_context *ctx, t_command_tree *node, char *eof)
{
	char	*new_line;
	int		pipe_fds[2];

	if (custom_pipe(ctx, pipe_fds))
		return ;
	add_int(&ctx->heredocs_fds, pipe_fds[0]);
	node->hd_fd = pipe_fds[0];
	new_line = read_line(ctx);
	while (new_line && ft_strncmp(new_line, eof, ft_strlen(eof) + 1))
	{
		new_line = expand_variables(ctx, new_line);
		ft_putendl_fd(new_line, pipe_fds[1]);
		free(new_line);
		new_line = read_line(ctx);
	}
	if (!new_line && !g_last_signal)
		heredoc_ending_error(ctx->shell_name, eof);
	free(new_line);
	close(pipe_fds[1]);
}

void	read_heredocs(t_context *ctx, t_command_tree *node)
{
	t_str_list	*token;

	if (node->sep != NONE)
	{
		read_heredocs(ctx, node->cmd1);
		read_heredocs(ctx, node->cmd2);
	}
	if (node->subshell)
	{
		token = node->subshell_redirections;
		read_heredocs(ctx, node->cmd1);
	}
	else
		token = node->cmd_tokens;
	while (token)
	{
		if (!ft_strncmp(token->content, "<<", 3))
			read_hd(ctx, node, token->next->content);
		token = token->next;
	}
}
