/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_signals.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 16:04:55 by rselva-2          #+#    #+#             */
/*   Updated: 2026/06/04 17:07:57 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

int	g_last_signal;

void	rl_handler_sigint(int sig)
{
	(void)sig;
	g_last_signal = sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	return ;
}

void	generic_handler_sigint(int sig)
{
	(void)sig;
	g_last_signal = sig;
	write(1, "\n", 1);
	return ;
}

void	set_default_signals(void)
{
	struct sigaction	sigquit_act;
	struct sigaction	sigint_act;

	sigint_act = (struct sigaction){.sa_handler = generic_handler_sigint};
	sigquit_act = (struct sigaction){.sa_handler = SIG_IGN};
	sigaction(SIGINT, &sigint_act, NULL);
	sigquit_act.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sigquit_act, NULL);
}
