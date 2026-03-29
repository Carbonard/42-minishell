/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_check_input.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 18:58:57 by rselva-2          #+#    #+#             */
/*   Updated: 2026/03/29 19:13:44 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

int	get_event(char *input, int i)
{
	if (input[i] == ' ')
		return (E_SPACE);
	else if (input[i] == '"')
		return (E_DOUBLE_QUOTE);
	else if (input[i] == '\'')
		return (E_SINGLE_QUOTE);
	else if (input[i] == '&' && input[i + 1] == '&' && input[i + 2] != '&')
		return (E_AND);
	else if (input[i] == '|' && input[i + 1] == '|' && input[i + 2] != '|')
		return (E_OR);
	else if (input[i] == '|' && input[i + 1] != '|')
		return (E_PIPE);
	else if (input[i] == '<' && input[i + 1] == '<' && input[i + 2] != '<')
		return (E_DOUBLE_REDIR_L);
	else if (input[i] == '>' && input[i + 1] == '>' && input[i + 2] != '>')
		return (E_DOUBLE_REDIR_R);
	else if (input[i] == '<' && input[i + 1] != '<')
		return (E_REDIR_L);
	else if (input[i] == '>' && input[i + 1] != '>')
		return (E_REDIR_R);
	else if (input[i] == '(')
		return (E_OPEN_PAR);
	else if (input[i] == ')')
		return (E_CLOSING_PAR);
	return (E_OTHER);
}

int	change_state(int current_state, int event)
{
						// E_SPACE,		 E_SINGLE_QUOTE,E_DOUBLE_QUOTE, E_AND,		E_OR,		 E_PIPE,	 E_REDIR_R,	 E_REDIR_L, E_DOUBLE_REDIR_R, E_DOUBLE_REDIR_L, E_OPEN_PAR,	 E_CLOSING_PAR, E_OTHER,
	static int const	conversor[S_TOTAL][E_TOTAL] = {
		[S_INITIAL] =	 {S_INITIAL,	 S_SING_QUOT,	S_DOUB_QUOT,	S_ERROR,	S_ERROR,	 S_ERROR,	 S_REDIR,	 S_REDIR,	 S_REDIR,		 S_REDIR,			 S_OPEN_PAR, S_ERROR,		 S_COMMAND},
		[S_SING_QUOT] =	 {S_SING_QUOT,	 S_LAST,	 	S_SING_QUOT,	S_SING_QUOT,S_SING_QUOT, S_SING_QUOT,S_SING_QUOT,S_SING_QUOT,S_SING_QUOT,	 S_SING_QUOT,		 S_SING_QUOT,S_SING_QUOT,	 S_SING_QUOT},
		[S_DOUB_QUOT] =	 {S_DOUB_QUOT,	 S_DOUB_QUOT,	S_LAST,			S_DOUB_QUOT,S_DOUB_QUOT, S_DOUB_QUOT,S_DOUB_QUOT,S_DOUB_QUOT,S_DOUB_QUOT,	 S_DOUB_QUOT,		 S_DOUB_QUOT,S_DOUB_QUOT,	 S_DOUB_QUOT},
		[S_CLOS_PAR] =	 {S_CLOS_PAR,	 S_ERROR,	 	S_ERROR,		S_INITIAL,	S_INITIAL,	 S_INITIAL,	 S_REDIR,	 S_REDIR,	 S_REDIR,		 S_REDIR,			 S_ERROR,	 S_LAST,		 S_ERROR},
		[S_REDIR] =		 {S_REDIR,		 S_SING_QUOT,	S_DOUB_QUOT,	S_ERROR,	S_ERROR,	 S_ERROR,	 S_ERROR,	 S_ERROR,	 S_ERROR,		 S_ERROR,			 S_ERROR,	 S_ERROR,		 S_READ_REDIR},
		[S_READ_REDIR] = {S_LAST,		 S_SING_QUOT,	S_DOUB_QUOT,	S_LAST,		S_LAST,		 S_LAST,	 S_REDIR,	 S_REDIR,	 S_REDIR,		 S_REDIR,			 S_ERROR,	 S_LAST,		 S_READ_REDIR},
		[S_COMMAND] =	 {S_COMMAND,	 S_SING_QUOT,	S_DOUB_QUOT,	S_INITIAL,	S_INITIAL,	 S_INITIAL,	 S_REDIR,	 S_REDIR,	 S_REDIR,		 S_REDIR,			 S_ERROR,	 S_LAST,		 S_COMMAND}
	};
	return (conversor[current_state][event]);
}

void	action(int last_state, int *state, int event, int *i)
{
	static int	saved_states[1000];
	static int	i_saved;

	if (event == E_AND || event == E_OR || event == E_DOUBLE_REDIR_L || event == E_DOUBLE_REDIR_R)
		(*i)++;
	if (*state == S_OPEN_PAR)
	{
		saved_states[i_saved] = S_CLOS_PAR;
		i_saved++;
		*state = S_INITIAL;
	}
	else if ((*state == S_SING_QUOT || *state == S_DOUB_QUOT) && *state != last_state)
	{
		saved_states[i_saved] = last_state;
		i_saved++;
	}
	else if (*state == S_REDIR && *state != last_state)
	{
		if (last_state == S_INITIAL)
			saved_states[i_saved] = S_COMMAND;
		else
			saved_states[i_saved] = last_state;
		i_saved++;
	}
	else if (*state == S_LAST)
	{
		if (i_saved == 0)
		{
			*state = S_ERROR;
			return ;
		}
		i_saved--;
		*state = saved_states[i_saved];
	}
}

int	check_input(char *input)
{
	int	i;
	int	last_state;
	int	state;
	int	event;

	state = S_INITIAL;
	i = 0;
	while (input[i] && state != S_ERROR)
	{
		event = get_event(input, i);
		// printf("1 state after reading %c: %i, %i\n", input[i], last_state, state);
		last_state = state;
		// printf("2 state after reading %c: %i, %i\n", input[i], last_state, state);
		state = change_state(state, event);
		// printf("3 state after reading %c: %i, %i\n", input[i], last_state, state);
		action(last_state, &state, event, &i);
		// printf("4 state after reading %c: %i, %i\n", input[i], last_state, state);
		i++;
	}
	if (state == S_ERROR || state == S_REDIR)
	{
		ft_putstr_fd("syntax error near unexpected token ", 2);
		ft_putchar_fd(input[i-1], 2);
		ft_putchar_fd('\n', 2);
		return (1);
	}
	return (0);
}
 