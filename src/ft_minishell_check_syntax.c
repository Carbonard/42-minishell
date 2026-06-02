/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_check_syntax.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 18:58:57 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/31 23:16:55 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_input.h"

static int	change_state(int current_state, int event)
{
	static int const	conversor[S_TOTAL][E_TOTAL] = {
	[S_INITIAL] = {S_INITIAL, S_ERROR, S_REDIR,
		S_INITIAL, S_ERROR, S_COMMAND, S_ERROR},
	[S_COMMAND] = {S_COMMAND, S_INITIAL, S_REDIR,
		S_ERROR, S_CLOS_PAR, S_COMMAND, S_ERROR},
	[S_CLOS_PAR] = {S_CLOS_PAR, S_INITIAL, S_REDIR_PAR,
		S_ERROR, S_CLOS_PAR, S_ERROR, S_ERROR},
	[S_REDIR] = {S_REDIR, S_ERROR, S_ERROR,
		S_ERROR, S_ERROR, S_COMMAND, S_ERROR},
	[S_REDIR_PAR] = {S_REDIR_PAR, S_ERROR, S_ERROR,
		S_ERROR, S_ERROR, S_CLOS_PAR, S_ERROR},
	};

	return (conversor[current_state][event]);
}

static int	get_event(char *token)
{
	if (!token)
		return (E_INVALID);
	else if (token[0] == '\n')
		return (E_NEW_LINE);
	else if ((token[0] == '&' && token[1] == '&')
		|| (token[0] == '|' && token[1] == '|')
		|| (token[0] == '|' && token[1] != '|'))
		return (E_OPERATOR);
	else if ((token[0] == '<' && token[1] == '<')
		|| (token[0] == '>' && token[1] == '>')
		|| (token[0] == '<' && token[1] != '<')
		|| (token[0] == '>' && token[1] != '>'))
		return (E_REDIR);
	else if (token[0] == '(')
		return (E_OPEN_PAR);
	else if (token[0] == ')')
		return (E_CLOSING_PAR);
	else if (ft_strchr(METACHARACTERS, token[0]))
		return (E_INVALID);
	return (E_OTHER);
}

static void	print_syntax_error(t_context *ctx, char *token)
{
	ft_putstr_fd(ctx->shell_name, 2);
	ft_putstr_fd(": syntax error near unexpected token `", 2);
	ft_putstr_fd(token, 2);
	ft_putendl_fd("'", 2);
	ctx->status = MS_E_SYNTAX;
	ctx->exit_status = 2;
}

int	check_syntax(t_context *ctx)
{
	t_str_list	*token;
	int			state;
	int			event;
	int			parenthesis;

	ctx->input_tokens = get_tokens(ctx->user_input, METACHARACTERS);
	if (!ctx->input_tokens)
		return (1);
	token = ctx->input_tokens;
	state = change_state(S_INITIAL, get_event(token->content));
	parenthesis = (get_event(token->content) == E_OPEN_PAR)
		- ((get_event(token->content) == E_CLOSING_PAR));
	while (token && token->next && state != S_ERROR && parenthesis >= 0)
	{
		token = token->next;
		event = get_event(token->content);
		parenthesis += (event == E_OPEN_PAR) - (event == E_CLOSING_PAR);
		state = change_state(state, event);
	}
	if (state == S_ERROR || parenthesis < 0
		|| state == S_REDIR || state == S_REDIR_PAR)
		print_syntax_error(ctx, token->content);
	return (ctx->status);
}
