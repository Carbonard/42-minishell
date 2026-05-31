/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_read_heredocs.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 17:22:13 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/30 23:40:19 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_input.h"
#include "ft_minishell_builtins.h"
#include "ft_minishell_execution.h"

// static char	*add_line(char *here_doc, char *new_line)
// {
// 	char	*aux;

// 	aux = ft_strjoin(here_doc, new_line);
// 	free(here_doc);
// 	free(new_line);
// 	return (aux);
// }

// static char	*read_hd_aux(
// 	t_context *ctx, char *here_doc, char *new_line, char *eof)
// {
// 	if (new_line)
// 	{
// 		here_doc = add_line(here_doc, new_line);
// 		here_doc[ft_strlen(here_doc) - 1] = 0;
// 	}
// 	else if (!ctx->no_tty && !g_last_signal)
// 	{
// 		ft_putchar_fd('\n', 2);
// 		ft_putstr_fd(ctx->shell_name, 2);
// 		ft_putstr_fd(
// 			": warning: here-document delimited by end-of-file (wanted `", 2);
// 		ft_putstr_fd(eof, 2);
// 		ft_putstr_fd("')\n", 2);
// 	}
// 	return (here_doc);
// }

// static char	*readline_hd()
// {
// 	struct sigaction	new_act;
// 	struct sigaction	old_act;
// 	char				*ret;

// 	ft_bzero(&new_act, sizeof(struct sigaction));
// 	new_act.sa_handler = generic_handler_sigint;
// 	sigaction(SIGINT, &new_act, &old_act);
// 	// ft_putstr_fd("> ", 1);
// 	ret = readline("> ");
// 	sigaction(SIGINT, &old_act, NULL);
// 	return (ret);
// }

// static void	read_hd(t_context *ctx, char *eof)
// {
// 	char	*here_doc;
// 	char	*new_line;
// 	int		len;

// 	len = ft_strlen(eof);
// 	here_doc = NULL;
// 	// if (!ctx->no_tty)
// 	// 	ft_putstr_fd("> ", 1);
// 	new_line = readline_hd();
// 	printf("a\n");
// 	while (new_line && !g_last_signal
// 		&& (ft_strncmp(new_line, eof, len) || new_line[len] != '\n'))
// 	{
// 		here_doc = add_line(here_doc, new_line);
// 		// if (!ctx->no_tty)
// 		// 	ft_putstr_fd("> ", 1);
// 		new_line = readline_hd();
// 	}
// 	here_doc = read_hd_aux(ctx, here_doc, new_line, eof);
// 	if (here_doc)
// 		add_ptr(&ctx->here_docs, here_doc);
// 	else
// 		add_ptr(&ctx->here_docs, ft_calloc(1, 1));
// 	add_ptr(&ctx->eofs, eof);
// }

// void	read_heredocs(t_context *ctx)
// {
// 	int		i;
// 	int		len;
// 	char	*eof;

// 	i = 0;
// 	while (ctx->user_input[i])
// 	{
// 		if (ctx->user_input[i] == '<' && ctx->user_input[i + 1] == '<')
// 		{
// 			while (ctx->user_input[i] == '<' || ctx->user_input[i] == ' ')
// 				i++;
// 			len = 0;
// 			while (ctx->user_input[i + len]
// 				&& !is_metachar(ctx->user_input[i + len]))
// 				len++;
// 			eof = ft_substr(ctx->user_input, i, len);
// 			read_hd(ctx, eof);
// 		}
// 		i++;
// 	}
// }

// static int	count_heredocs(t_str_list *token)
// {
// 	int	cnt;

// 	cnt = 0;
// 	while (token)
// 	{
// 		if (!ft_strncmp(token->content, "<<", 3))
// 			cnt++;
// 		token = token->next;
// 	}
// 	return (cnt);
// }

static char	*read_line(t_context *ctx)
{
	char	*line;

	if (!ctx->no_tty)
		ft_putstr_fd("> ", 1);
	line = get_next_line(0);
	if (line && line [0] && line[ft_strlen(line) - 1] == '\n')
		line[ft_strlen(line) - 1] = 0;
	return (line);
}

static void	read_hd(t_context *ctx, t_command_tree *node, char *eof)
{
	char	*new_line;
	int		pipe_fds[2];

	pipe(pipe_fds);
	add_int(&ctx->heredocs_fds, pipe_fds[0]);
	node->hd_fd = pipe_fds[0];
	new_line = read_line(ctx);
	while (new_line && ft_strncmp(new_line, eof, ft_strlen(eof) + 1))
	{
		new_line = expand_input(ctx, new_line);
		ft_putendl_fd(new_line, pipe_fds[1]);
		free(new_line);
		new_line = read_line(ctx);
	}
	if (!new_line && !g_last_signal)
	{
		ft_putchar_fd('\n', 2);
		ft_putstr_fd(ctx->shell_name, 2);
		ft_putstr_fd(
			": warning: here-document delimited by end-of-file (wanted `", 2);
		ft_putstr_fd(eof, 2);
		ft_putstr_fd("')\n", 2);
	}
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
		token = node->subshell_redirections;
	else
		token = node->cmd_tokens;
	while (token)
	{
		if (!ft_strncmp(token->content, "<<", 3))
			read_hd(ctx, node, token->next->content);
		token = token->next;
	}
}
