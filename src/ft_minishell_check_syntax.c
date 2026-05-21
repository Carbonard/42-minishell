/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_check_syntax.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 18:58:57 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/21 01:24:11 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_input.h"

static int	change_state(int current_state, int event)
{
						// E_OPERATOR,	E_REDIR,	E_OPEN_PAR,	 E_CLOS_PAR, E_OTHER,	E_INVALID
	static int const	conversor[S_TOTAL][E_TOTAL] = {
		[S_INITIAL] =	 {S_ERROR,		S_REDIR,	S_INITIAL,	S_ERROR,	S_COMMAND,	S_ERROR},
		[S_COMMAND] =	 {S_INITIAL,	S_REDIR,	S_ERROR,	S_CLOS_PAR,	S_COMMAND,	S_ERROR},
		[S_CLOS_PAR] =	 {S_INITIAL,	S_REDIR_PAR,S_ERROR,	S_CLOS_PAR,	S_ERROR,	S_ERROR},
		[S_REDIR] =		 {S_ERROR,		S_ERROR,	S_ERROR,	S_ERROR,	S_COMMAND,	S_ERROR},
		[S_REDIR_PAR] =	 {S_ERROR,		S_ERROR,	S_ERROR,	S_ERROR,	S_CLOS_PAR,	S_ERROR},
	};
	return (conversor[current_state][event]);
}

static int	get_event(char *token)
{
	if ((token[0] == '&' && token[1] == '&')
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
	else if (is_metachar(token[0]))
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

	if (get_tokens(ctx))
		return (1);
	token = ctx->input_tokens;
	state = S_INITIAL;
	parenthesis = 0;
	while (state != S_ERROR)
	{
		event = get_event(token->content);
		if (event == E_OPEN_PAR)
			parenthesis++;
		else if (event == E_CLOSING_PAR)
			parenthesis--;
		state = change_state(state, event);
		if (!(token->next) || state == S_ERROR || parenthesis < 0)
			break ;
		token = token->next;
	}
	if (state == S_ERROR || state == S_REDIR || parenthesis < 0)
		print_syntax_error(ctx, token->content);
	return (ctx->status);
}
