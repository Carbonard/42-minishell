/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_get_input.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 20:24:41 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/18 20:25:09 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

static int	get_prompt(char *prompt)
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

static int	open_quotes(char *input)
{
	int		i;
	char	quote;

	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			quote = input[i];
			i++;
			while (input[i] && input[i] != quote)
				i++;
			if (!input[i])
				return (1);
		}
		i++;
	}
	return (0);
}

char	*read_input(void)
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
