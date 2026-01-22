/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 18:55:11 by rselva-2          #+#    #+#             */
/*   Updated: 2026/01/22 16:59:40 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

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

int	export(t_context *ctx, char *new_var)
{
	t_str_list	*item;

	item = find_env_node(ctx, new_var);
	if (item)
	{
		free(item->content);
		item->content = ft_strdup(new_var);
		if (!new_var)
			return (MS_E_ENV_MALLOC);
		return (MS_SUCCESS);
	}
	return (add_env(ctx, new_var));
}

int	unset(t_context *ctx, char *var)
{
	t_str_list	*item;

	item = find_env_node(ctx, var);
	if (item)
	{
		ft_str_delitem(&ctx->env, item);
		return (MS_SUCCESS);
	}
	return (MS_E_VAR_NFOUND);
}
