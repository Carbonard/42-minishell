/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_split_cmd.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 17:03:15 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/17 21:14:37 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

static void	cut_string(char *cmd)
{
	char		quote;
	int			shift;

	shift = 0;
	quote = 0;
	while (*cmd)
	{
		if (!quote && (*cmd == '\'' || *cmd == '"'))
		{
			quote = *cmd;
			shift++;
		}
		else if (quote || *cmd != ' ')
		{
			*(cmd - shift) = *cmd;
			if (*cmd == quote)
				shift++;
			if (*cmd == quote)
				quote = 0;
		}
		else
			*(cmd - shift) = 0;
		cmd++;
	}
	ft_bzero(cmd - shift, shift);
}

char	**split_cmd(char *cmd)
{
	t_dyn_ptr	split;
	int			length;
	int			i;

	init_dyn_ptr(&split, 2);
	length = ft_strlen(cmd);
	cut_string(cmd);
	i = 0;
	while (i < length)
	{
		if (cmd[i] && (i == 0 || cmd[i-1] == 0))
			add_ptr(&split, cmd + i);
		i++;
	}
	add_ptr(&split, NULL);
	return (split.arr);
}

// char	**split_cmd(char *cmd)
// {
// 	t_dyn_ptr	split;
// 	char		*arg_start;
// 	char		quote;
// 	int			i;
// 	int			shift;

// 	i = 0;
// 	shift = 0;
// 	quote = 0;
// 	init_dyn_ptr(&split, 2);
// 	arg_start = NULL;
// 	while (cmd[i])
// 	{
// 		if (!arg_start && cmd[i] != ' ')
// 			arg_start = cmd + i - shift;
// 		if (!quote && (cmd[i] == '\'' || cmd[i] == '"'))
// 		{
// 			quote = cmd[i];
// 			shift++;
// 		}
// 		else if (quote || cmd[i] != ' ')
// 		{
// 			cmd[i - shift] = cmd[i];
// 			if (cmd[i] == quote)
// 			{
// 				quote = 0;
// 				shift++;
// 			}
// 		}
// 		else if (arg_start)
// 		{
// 			cmd[i - shift] = 0;
// 			add_ptr(&split, arg_start);
// 			arg_start = NULL;
// 		}
// 		i++;
// 	}
// 	if (arg_start)
// 	{
// 		cmd[i - shift] = 0;
// 		add_ptr(&split, arg_start);
// 	}
// 	add_ptr(&split, NULL);
// 	return (split.arr);
// }

// char	**split_cmd(char *cmd)
// {
// 	t_dyn_ptr	split;
// 	char		*arg_start;
// 	char		quote;
// 	int			i;
// 	int			shift;

// 	i = 0;
// 	shift = 0;
// 	while (cmd[i])
// 	{
// 		arg_start = cmd + i;
// 		if (cmd[i] == '\'' || cmd[i] == '"')
// 		{
// 			quote = cmd[i];
// 			shift++;
// 			while (cmd[i] && cmd[i] != quote)
// 			{
// 				i++;
// 				cmd[i - shift] = cmd[i];
// 			}
// 			cmd[i - shift + 1] = 0;
// 		}
// 	}
// }

// estados	comilla	espacio	letra 

// inicial	comilla	inicial	texto
// comilla	texto	comilla	comilla
// texto	comilla	