/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_builtins_export.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elangari <elangari@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 15:51:24 by elangari          #+#    #+#             */
/*   Updated: 2026/05/31 17:49:14 by elangari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_minishell_builtins.h"


// create array
// order array
// print array

static void order_export(t_dyn_ptr *exp)
{
	size_t	i;
	size_t	si;
	char	*tmp;

	i = 0;
	si = 1;
	while (i < exp->length)
	{
		while ((si) < exp->length)
		{
			if (ft_strncmp(exp->arr[i], exp->arr[si], ft_strlen(exp->arr[i])) > 0)
			{
				tmp = exp->arr[i];
				exp->arr[i] = exp->arr[si];
				exp->arr[si] = tmp;
			}
			si++;
		}
		i++;
		si = i + 1;
	}
	return ;
}

void	print_export(t_context *ctx)
{
	t_dyn_ptr	exp;
	t_str_list	*env;
	size_t		i;

	i = 0;
	env = ctx->env;
	init_dyn_ptr(&exp, 1);
	while (env)
	{
		add_ptr(&exp, env->content);
		env = env->next;
	}
	order_export(&exp);
	while (i < exp.length)
	{
		printf("export %s\n", exp.arr[i]);
		i++;
	}
	free(exp.arr);
	return ;
}
