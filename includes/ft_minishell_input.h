/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_input.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 18:09:40 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/26 12:53:31 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

#define MAX_PROMPT 4126

enum e_automaton_events
{
	E_SPACE,
	E_SINGLE_QUOTE,
	E_DOUBLE_QUOTE,
	E_AND,
	E_OR,
	E_PIPE,
	E_REDIR_R,
	E_REDIR_L,
	E_DOUBLE_REDIR_R,
	E_DOUBLE_REDIR_L,
	E_OPEN_PAR,
	E_CLOSING_PAR,
	E_OTHER,
	E_TOTAL
};

enum e_automaton_states
{
	S_INITIAL,
	S_COMMAND,
	S_SING_QUOT,
	S_DOUB_QUOT,
	S_OPEN_PAR,
	S_CLOS_PAR,
	S_REDIR,
	S_READ_REDIR,
	S_ERROR,
	S_LAST,
	S_TOTAL
};

typedef struct s_automaton_data
{
	int		state;
	int		last_state;
	int		event;
	int		i_saved;
	char	saved_states[1000];
}	t_automaton_data;

// heredoc
void	read_heredocs(t_context *ctx);
void	expand_heredoc(t_context *ctx);
int		spread_heredocs(
			t_command_tree *node, t_dyn_ptr *hd, t_dyn_ptr *eof, int n);
// get_input
int		read_input(t_context *ctx);
// check_syntax
int		check_syntax(t_context *ctx);
// check_input
int		check_quotes(t_context *ctx);
int		check_parenthesis(t_context *ctx);
int		check_operator(t_context *ctx);

int		advance_quotes(char *str);
