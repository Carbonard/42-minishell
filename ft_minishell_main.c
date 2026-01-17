/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_main.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 14:59:51 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/17 20:18:01 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>


int	get_prompt(char *prompt)
{
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
		return (ft_strlen(home) - 1);
	}
	else
		return (0);
}

int	open_quotes(char *input)
{
	int		i;
	char	quote;

	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			// printf("\n(%c)\n", input[i]);
			quote = input[i];
			i++;
			while (input[i] && input[i] != quote)
				i++;
			if (!input[i])
				return (1);
		}
		// else
		// 	putchar(input[i]);
		i++;
	}
	return (0);
}

char	*read_input()
{
	char	prompt[MAX_PWD + 3];
	char	*input;
	char	*input_extension;
	char	*aux;
	int		start;

	start = get_prompt(prompt);
	input = readline(prompt + start);
	while (open_quotes(input))
	{
		input_extension = readline("> ");
		aux = ft_strjoin_char(input, input_extension, '\n');
		free(input);
		input = aux;
	}
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
		command = split_cmd(input);
		if (!ft_strncmp(command[0], "cd", 3))
			chdir(command[1]);
		else if (!ft_strncmp(command[0], "pwd", 4))
			pwd();
		else if (!ft_strncmp(command[0], "env", 4))
			print_env(env);
		else if (!ft_strncmp(command[0], "echo", 7))
			echo(command);
		free(input);
		free(command);
	}
(void)argc;(void)argv;(void)env;
// 	pwd();
// 	chdir("../07_pipex");
// 	pwd();
	return (0);
}
