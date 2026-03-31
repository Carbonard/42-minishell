#include "ft_minishell.h"

int	last_signal;

void	handler_sigint(int sig)
{
	(void)sig;
	last_signal = sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	return ;
}
