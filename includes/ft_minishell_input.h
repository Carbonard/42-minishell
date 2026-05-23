/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_input.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 18:09:40 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/21 19:50:17 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

#define MAX_PROMPT 4126

enum e_automaton_events
{
	E_NEW_LINE,
	E_OPERATOR,
	E_REDIR,
	E_OPEN_PAR,
	E_CLOSING_PAR,
	E_OTHER,
	E_INVALID,
	E_TOTAL
};

enum e_automaton_states
{
	S_INITIAL,
	S_COMMAND,
	S_CLOS_PAR,
	S_REDIR,
	S_REDIR_PAR,
	S_ERROR,
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
void	str_replace(char *str, char *find, char *subst);
void	escape_text(char *str);
void	get_prompt(t_context *ctx, char *prompt);
int		read_input(t_context *ctx);
// tokens
t_str_list	*get_tokens(char *input);
// check_syntax
int		check_syntax(t_context *ctx);
// check_input
int		check_quotes(t_context *ctx);
int		check_parenthesis(t_context *ctx);
int		check_operator(t_context *ctx);
char	*remove_quotes(char *cmd);
int		advance_quotes(char *str);
