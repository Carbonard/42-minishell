#include "ft_minishell.h"

void	handler_sigint(int sig)
{
	(void)sig;
	// write(1, "\n", 1);
	// rl_replace_line("", 0);
	// rl_on_new_line();
	// rl_redisplay();
	return ;
}
