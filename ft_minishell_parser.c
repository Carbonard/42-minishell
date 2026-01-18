/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_parser.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyxssa <nyxssa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 16:54:26 by nyxssa            #+#    #+#             */
/*   Updated: 2026/01/18 16:16:35 by nyxssa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"


static int		find_closing_par(char *str)
{
	int	parenthesis_checker;
	int	parenthesis_checker_i;

	parenthesis_checker = 1;
	parenthesis_checker_i = 0;
	while (parenthesis_checker != 0)
		{
			parenthesis_checker_i++;
			if (str[parenthesis_checker_i] == '(')	
				parenthesis_checker++;
			else if (str[parenthesis_checker_i] == ')')	
				parenthesis_checker--;
		}
	return (parenthesis_checker_i);
}

void	remove_parenthesis(char *str)
{
	int parenthesis_checker_i;
	int	space_cnt;

	while (*str == ' ' || *str == '(')
	{
		space_cnt = 1;
		if (*str == '(')
		{
			parenthesis_checker_i = find_closing_par(str);
			while (str[parenthesis_checker_i + space_cnt] == ' ')
				space_cnt++;
			if (str[parenthesis_checker_i + space_cnt] == 0)
			{
				str[0] = ' ';
				str[parenthesis_checker_i] = ' ';
			}
		}
		str++;
	}
}

// void	create_nodes(t_command *first, t_command *second, int i, int step)
// {
// 			input->cmd[i] = 0;
// 			first = malloc(sizeof(t_command));
// 			second = malloc(sizeof(t_command));
// 			first->cmd = input->cmd;
// 			second->cmd = input->cmd + i + step;
// 			input->cmd1 = first;
// 			input->cmd2 = second;
// 			divide_by_logic_op(first);
// 			divide_by_logic_op(second);
// 			return ;
// }

// void	create_nodes(t_command **first, t_command **second, char **left, char **right)
// {
// 			*first = malloc(sizeof(t_command));
// 			*second = malloc(sizeof(t_command));
// 			(*first)->cmd = left;
// 			(*second)->cmd = right;
// 			divide_by_logic_op(first);
// 			divide_by_logic_op(second);
// 			return ;
// }


static int	divide_by_logic_op(t_command *input, t_command **first, t_command **second)
{
	int			i;
printf("ands\n");
	i = 0;
	remove_parenthesis(input->cmd);
	while (input->cmd[i] != 0)
	{
		if ((input->cmd[i] == '&' && input->cmd[i+1] == '&')
			|| (input->cmd[i] == '|' && input->cmd[i+1] == '|'))
		{
			if (input->cmd[i] == '&')
				input->sep = AND;
			else
				input->sep = OR;
			input->cmd[i] = 0;
			*first = malloc(sizeof(t_command));
			*second = malloc(sizeof(t_command));
			(*first)->cmd = input->cmd;
			(*second)->cmd = input->cmd + i + 2;
			input->cmd1 = *first;
			input->cmd2 = *second;
			return (1);
		}
		i++;
	}
	return (0);
}

// static int	divide_by_ors(t_command *input, t_command **first, t_command **second)
// {
// 	int			i;
// printf("ors\n");
// 	i = 0;
// 	remove_parenthesis(input->cmd);
// 	while (input->cmd[i] != 0)
// 	{
// 		if (input->cmd[i] == '|' && input->cmd[i+1] == '|')
// 		{
// 			input->sep = OR;
// 			input->cmd[i] = 0;
// 			*first = malloc(sizeof(t_command));
// 			*second = malloc(sizeof(t_command));
// 			(*first)->cmd = input->cmd;
// 			(*second)->cmd = input->cmd + i + 2;
// 			input->cmd1 = *first;
// 			input->cmd2 = *second;
// 			return (1);
// 		}
// 		i++;
// 	}
// 	return (0);
// }

static int	divide_by_pipes(t_command *input, t_command **first, t_command **second)
{
	int			i;
printf("pipes\n");
	i = 0;
	remove_parenthesis(input->cmd);
	while (input->cmd[i] != 0)
	{
		if (input->cmd[i] == '|')
		{
			input->sep = PIPE;
			input->cmd[i] = 0;
			*first = malloc(sizeof(t_command));
			*second = malloc(sizeof(t_command));
			(*first)->cmd = input->cmd;
			(*second)->cmd = input->cmd + i + 1;
			input->cmd1 = *first;
			input->cmd2 = *second;
			return (1);
		}
		i++;
	}
	return (0);
}

void	create_tree(t_command *input)
{
	t_command	*first;
	t_command	*second;

	first = NULL;
	second = NULL;
	remove_parenthesis(input->cmd);
	if (!divide_by_logic_op(input, &first, &second))
	// if (!divide_by_ors(input, &first, &second))
		divide_by_pipes(input, &first, &second);
	input->cmd1 = first;
	input->cmd2 = second;
	if (first)
	{
		create_tree(first);
		create_tree(second);
	}
}

void	display_tree(t_command *tree)
{
	if (!tree->cmd1)
	{
		printf("[%s]", tree->cmd);
		return;
	}
	printf("[ ");
	display_tree(tree->cmd1);
	printf(" %d ", tree->sep);
	display_tree(tree->cmd2);
	printf(" ]");	
}

// void	divide_by_logic_op(t_command *input)
// {
// 	t_command	*first;
// 	t_command	*second;
// 	int			i;

// 	i = 0;
// 	remove_parenthesis(input->cmd);
// 	while (input->cmd[i] != 0)
// 	{
// 		if (input->cmd[i] == '&' && input->cmd[i+1] == '&')
// 		{
// 			input->sep = AND;
// 			input->cmd[i] = 0;
// 			first = malloc(sizeof(t_command));
// 			second = malloc(sizeof(t_command));
// 			first->cmd = input->cmd;
// 			second->cmd = input->cmd + i + 2;
// 			input->cmd1 = first;
// 			input->cmd2 = second;
// 			divide_by_logic_op(first);
// 			divide_by_logic_op(second);
// 			return ;
// 		}
// 		i++;
// 	}
// }

/*int		main()
{
	char	*star;

	star = ft_strdup("(( ( (hola)(que tal)  ) ) ) ");
	remove_parenthesis(star);
	ft_printf("%s\n\n\n", star);
}*/
