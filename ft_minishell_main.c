/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_main.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 14:59:51 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/17 03:46:55 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	display_prompt()
{
	char	prompt[MAX_PWD + 3];
	char	home[100];
	int		length;

	getcwd(prompt, MAX_PWD);
	length = ft_strlen(prompt);
	prompt[length] = '$';
	prompt[length + 1] = ' ';
	prompt[length + 2] = 0;
	ft_strlcpy(home, "/home/", 100);
	ft_strlcat(home, getenv("USER"), 100);
	if (!ft_strncmp(prompt, home, ft_strlen(home)))
	{
		prompt[ft_strlen(home) - 1] = '~';
		ft_putstr_fd(prompt + ft_strlen(home) - 1, 1);
	}
	else
		ft_putstr_fd(prompt, 1);
}

char	*read_input()
{
	char	*input;

	display_prompt();
	input = get_next_line(0);
	input[ft_strlen(input) - 1] = 0;
	return (input);
}

int	main(int argc, char **argv, char **env)
{
	int		status;
	char	*input;
	char	**command;

	status = 1;
	while (status)
	{
		input = read_input();
		command = ft_split(input, ' ');
		if (!ft_strncmp(command[0], "cd", 3))
			chdir(command[1]);
		else if (!ft_strncmp(command[0], "pwd", 4))
			pwd();
		else if (!ft_strncmp(command[0], "env", 4))
			print_env(env);
		else if (!ft_strncmp(command[0], "echo", 7))
			echo(command);
	}
(void)argc;(void)argv;(void)env;
// 	pwd();
// 	chdir("../07_pipex");
// 	pwd();
	return (0);
}
