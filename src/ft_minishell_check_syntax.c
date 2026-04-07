/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_check_syntax.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 18:58:57 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/07 22:44:06 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_input.h"

static int	get_event(char *input, int i)
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

static int	change_state(int current_state, int event)
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

static void	action_2(t_automaton_data *data)
{
	if (data->state == S_REDIR && data->state != data->last_state)
	{
		if (data->last_state == S_INITIAL)
			data->saved_states[data->i_saved] = S_COMMAND;
		else
			data->saved_states[data->i_saved] = data->last_state;
		data->i_saved++;
	}
	else if (data->state == S_LAST)
	{
		if (data->i_saved == 0)
		{
			data->state = S_ERROR;
			return ;
		}
		data->i_saved--;
		data->state = data->saved_states[data->i_saved];
	}
}

static void	action(t_automaton_data *data, int *i)
{
	if (data->event == E_AND || data->event == E_OR || data->event == E_DOUBLE_REDIR_L || data->event == E_DOUBLE_REDIR_R)
		(*i)++;
	if (data->state == S_OPEN_PAR)
	{
		data->saved_states[data->i_saved] = S_CLOS_PAR;
		data->i_saved++;
		data->state = S_INITIAL;
	}
	else if ((data->state == S_SING_QUOT || data->state == S_DOUB_QUOT) && data->state != data->last_state)
	{
		data->saved_states[data->i_saved] = data->last_state;
		data->i_saved++;
	}
	else
		action_2(data);
}

int	check_syntax(char *input)
{
	int					i;
	t_automaton_data	data;

	data.state = S_INITIAL;
	data.last_state = S_INITIAL;
	data.i_saved = 0;
	i = 0;
	while (input[i] && data.state != S_ERROR)
	{
		data.event = get_event(input, i);
		// printf("1 state after reading %c: %i, %i\n", input[i], last_state, state);
		data.last_state = data.state;
		// printf("2 state after reading %c: %i, %i\n", input[i], last_state, state);
		data.state = change_state(data.state, data.event);
		// printf("3 state after reading %c: %i, %i\n", input[i], last_state, state);
		action(&data, &i);
		// printf("4 state after reading %c: %i, %i\n", input[i], last_state, state);
		i++;
	}
	if (data.state == S_ERROR || data.state == S_REDIR)
	{
		ft_putstr_fd("syntax error near unexpected token ", 2);
		ft_putchar_fd(input[i-1], 2);
		ft_putchar_fd('\n', 2);
		return (1);
	}
	return (0);
}
 