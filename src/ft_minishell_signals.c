/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_signals.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 16:04:55 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/11 19:47:46 by rselva-2         ###   ########.fr       */
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
	struct sigaction	act;
	struct sigaction	actt;

	actt = (struct sigaction){.sa_handler = generic_handler_sigint};
	act = (struct sigaction){.sa_handler = SIG_IGN};
	sigaction(SIGINT, &actt, NULL);
	act.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &act, NULL);
}
