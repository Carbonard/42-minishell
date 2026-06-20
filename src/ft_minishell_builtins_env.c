/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_builtins_env.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 18:55:11 by rselva-2          #+#    #+#             */
/*   Updated: 2026/06/20 22:03:58 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_builtins.h"

int	print_env(t_context *ctx)
{
	t_str_list	*item;

	if (!ctx->env)
		return (MS_E_ENV_NFOUND);
	item = ctx->env;
	while (item)
	{
		ft_putendl_fd(item->content, 1);
		item = item->next;
	}
	return (MS_SUCCESS);
}

int	unset(t_context *ctx, char **argv)
{
	t_str_list	*item;
	char		*var;
	int			arg_i;

	arg_i = 1;
	while (argv[arg_i])
	{
		var = argv[arg_i];
		if (!ft_strchr(var, '='))
		{
			item = find_env_node(ctx, var);
			if (item)
				ft_str_delitem(&ctx->env, item);
		}
		arg_i++;
	}
	return (MS_SUCCESS);
}
