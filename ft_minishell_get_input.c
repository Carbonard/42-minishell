/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_get_input.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 20:24:41 by rselva-2          #+#    #+#             */
/*   Updated: 2026/03/24 17:43:56 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

int	get_prompt(char *prompt)
{
	char	home[100];
	char	pwd[MAX_PWD];

	ft_strlcpy(home, "/home/", 100);
	ft_strlcat(home, getenv("USER"), 100);
	getcwd(pwd, MAX_PWD);
	ft_strlcpy(prompt, "\e[94m", MAX_PROMPT);
	if (!ft_strncmp(home, pwd, ft_strlen(home)))
	{
		ft_strlcat(prompt, "~", MAX_PROMPT);
		ft_strlcat(prompt, pwd + ft_strlen(home), MAX_PROMPT);
	}
	else
		ft_strlcat(prompt, pwd, MAX_PROMPT);
	ft_strlcat(prompt, "$ \e[0m", MAX_PROMPT);
}

static int	check_quotes(t_context *ctx)
{
	int		i;
	char	quote;

	i = 0;
	while (ctx->user_input[i])
	{
		if (ctx->user_input[i] == '\'' || ctx->user_input[i] == '"')
		{
			quote = ctx->user_input[i];
			i++;
			while (ctx->user_input[i] && ctx->user_input[i] != quote)
				i++;
			if (!ctx->user_input[i])
			{
				ctx->status = MS_OPENQUOTE;
				return (1);
			}
		}
		i++;
	}
	return (0);
}

static int	check_parenthesis(t_context *ctx)
{
	int	i;
	int	parenthesis_checker;

	i = 0;
	parenthesis_checker = 0;
	while (ctx->user_input[i])
	{
		if (ctx->user_input[i] == '(')
			parenthesis_checker++;
		if (ctx->user_input[i] == ')')
			parenthesis_checker--;
		if (parenthesis_checker < 0)
		{
			ctx->status = MS_E_SYNTAX;
			return (1);
		}
		i++;
	}
	if (parenthesis_checker != 0)
		return (1);
	return (0);
}

static int	is_operator(char *str)
{
	if ((str[0] == '&' && str[1] == '&') || (str[0] == '|' && str[1] == '|'))
		return (2);
	if (str[0] == '|')
		return (1);
	return (0);
}

static int	is_redirection(char *str)
{
	if ((str[0] == '<' && str[1] == '<') || (str[0] == '>' && str[1] == '>'))
		return (2);
	if (str[0] == '<' || str[0] == '>')
		return (1);
	return (0);
}

static int	check_operator(t_context *ctx)
{
	int	i;

	i = 0;
	while (ctx->user_input[i])
	{
		if (is_operator(ctx->user_input + i))
		{
// printf("1.%s\n", ctx->user_input + i);
			i += is_operator(ctx->user_input + i);
// printf("2.%s\n", ctx->user_input + i);
			while ((ctx->user_input[i] == ' ' || ctx->user_input[i] == '\n'))
				i++;
// printf("3.%s\n", ctx->user_input + i);
			if (ctx->user_input[i] == 0)
			{
				ctx->status = MS_E_SYNTAX;
				return (1);
			}
		}
		else if (is_redirection(ctx->user_input + i))
		{
			i += is_redirection(ctx->user_input + i);
			while ((ctx->user_input[i] == ' '))
				i++;
			if (ctx->user_input == 0)
			{
				ctx->status = MS_E_SYNTAX_EOF;
				return (1);
			}
		}
		i++;
	}
	return (0);
}

int	get_event(char *input, int ind)
{
	if (input[ind] == ' ')
		return (E_SPACE);
	else if (input[ind] == '"')
		return (E_DOUBLE_QUOTE);
	else if (input[ind] == '\'')
		return (E_SINGLE_QUOTE);
	else if (input[ind] == '&' && input[ind + 1] == '&' && input[ind + 2] != '&')
		return (E_AND);
	else if (input[ind] == '|' && input[ind + 1] == '|' && input[ind + 2] != '|')
		return (E_OR);
	else if (input[ind] == '|' && input[ind + 1] != '|')
		return (E_PIPE);
	else if (input[ind] == '<' && input[ind + 1] == '<' && input[ind + 2] != '<')
		return (E_DOUBLE_REDIR_L);
	else if (input[ind] == '>' && input[ind + 1] == '>' && input[ind + 2] != '>')
		return (E_DOUBLE_REDIR_R);
	else if (input[ind] == '<' && input[ind + 1] != '<')
		return (E_REDIR_L);
	else if (input[ind] == '>' && input[ind + 1] != '>')
		return (E_REDIR_R);
	else if (input[ind] == '(')
		return (E_OPEN_PAR);
	else if (input[ind] == ')')
		return (E_CLOSING_PAR);
	return (E_OTHER);
}

int	change_state(int current_state, int event)
{
						// E_SPACE, E_SINGLE_QUOTE, E_DOUBLE_QUOTE, E_AND, E_OR, E_PIPE, E_REDIR_R, E_REDIR_L, E_DOUBLE_REDIR_R, E_DOUBLE_REDIR_L, E_OPEN_PAR, E_CLOSING_PAR, E_OTHER,
	static int const	conversor[S_TOTAL][E_TOTAL] = {
		[S_INITIAL] =	 {S_INITIAL, S_SING_QUOT, S_DOUB_QUOT, S_ERROR, S_ERROR, S_ERROR, S_REDIR, S_REDIR, S_REDIR, S_REDIR, S_OPEN_PAR, S_ERROR, S_COMMAND},
		[S_SING_QUOT] =	 {S_SING_QUOT, S_LAST, S_SING_QUOT, S_SING_QUOT, S_SING_QUOT, S_SING_QUOT, S_SING_QUOT, S_SING_QUOT, S_SING_QUOT, S_SING_QUOT, S_SING_QUOT, S_SING_QUOT, S_SING_QUOT},
		[S_DOUB_QUOT] =	 {S_DOUB_QUOT, S_DOUB_QUOT, S_LAST, S_DOUB_QUOT, S_DOUB_QUOT, S_DOUB_QUOT, S_DOUB_QUOT, S_DOUB_QUOT, S_DOUB_QUOT, S_DOUB_QUOT, S_DOUB_QUOT, S_DOUB_QUOT, S_DOUB_QUOT},
		[S_CLOS_PAR] =	 {S_CLOS_PAR, S_ERROR, S_ERROR, S_INITIAL, S_INITIAL, S_INITIAL, S_REDIR, S_REDIR, S_REDIR, S_REDIR, S_ERROR, S_LAST, S_ERROR},
		[S_REDIR] =		 {S_REDIR, S_SING_QUOT, S_DOUB_QUOT, S_ERROR, S_ERROR, S_ERROR, S_ERROR, S_ERROR, S_ERROR, S_ERROR, S_ERROR, S_ERROR, S_READ_REDIR},
		[S_READ_REDIR] = {S_LAST, S_SING_QUOT, S_DOUB_QUOT, S_ERROR, S_ERROR, S_ERROR, S_REDIR, S_REDIR, S_REDIR, S_REDIR, S_ERROR, S_ERROR, S_READ_REDIR},
		[S_COMMAND] =	 {S_COMMAND, S_SING_QUOT, S_DOUB_QUOT, S_INITIAL, S_INITIAL, S_INITIAL, S_REDIR, S_REDIR, S_REDIR, S_REDIR, S_ERROR, S_LAST, S_COMMAND}
	};
	return (conversor[current_state][event]);
}

void	action(int last_state, int *state, int event, int *i)
{
	static int	saved_states[1000];
	static int	i_saved;

	if (event == E_AND || event == E_OR || event == E_DOUBLE_REDIR_L || event == E_DOUBLE_REDIR_R)
		(*i)++;
	else if (*state == S_OPEN_PAR)
	{
		saved_states[i_saved] = S_CLOS_PAR;
		i_saved++;
		*state = S_INITIAL;
	}
	else if ((*state == S_SING_QUOT || *state == S_DOUB_QUOT) && *state != last_state)
	{
		saved_states[i_saved] = last_state;
		// printf("no\n");
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
			// printf("Si\n");
			*state = S_ERROR;
			return ;
		}
		i_saved--;
		*state = saved_states[i_saved];
	}
	// printf("wtf state: %d, last: %d\n", *state, last_state);
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

void	read_input(t_context *ctx)
{
	char	prompt[MAX_PROMPT];
	char	*input_extension;
	char	*aux;
	// int		start;

	ctx->status = MS_SUCCESS;
	get_prompt(prompt);
	if (ctx->interactive)
		ctx->user_input = get_next_line(STDIN_FILENO);
	else
	{
		ctx->user_input = readline(prompt);
	}
	if (!ctx->user_input)
	{
		ctx->user_input = ft_strdup("exit");
		if (!ctx->interactive)
			printf("exit\n");
		return ;
	}
	if (check_input(ctx->user_input))
	{
		free(ctx->user_input);
		ctx->user_input = ft_calloc(1,1);
		return ;
	}
	while ((check_quotes(ctx) || check_parenthesis(ctx) || check_operator(ctx)))
	{
		if (ctx->interactive)
		{
			input_extension = get_next_line(STDIN_FILENO);
			//Checkear errores de malloc
			aux = ft_strjoin(ctx->user_input, input_extension);
		}
		else
		{
			input_extension = readline("> ");
			aux = ft_strjoin_char(ctx->user_input, input_extension, '\n');
		}
		free(ctx->user_input);
		ctx->user_input = aux;
	}
	add_history(ctx->user_input);
}
