/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_environment.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 22:18:08 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/03 18:55:33 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_builtins.h"

int	save_env(t_context *ctx, char **original_env)
{
	ctx->env = NULL;
	while (*original_env)
	{
		if (add_env(ctx, *original_env) != MS_SUCCESS)
			return (MS_E_ENV_MALLOC);
		original_env++;
	}
	return (MS_SUCCESS);
}

int	add_env(t_context *ctx, char *new_var)
{
	t_str_list	*new_item;

	new_var = ft_strdup(new_var);
	if (!new_var)
		return (MS_E_ENV_MALLOC);
	new_item = ft_str_lstnew(new_var);
	if (!new_item)
		return (MS_E_ENV_MALLOC);
		//Borrar todo?
	ft_str_lstadd_back(&ctx->env, new_item);
	return (MS_SUCCESS);
}

// This function can receive VAR=value or just VAR
t_str_list	*find_env_node(t_context *ctx, char *var)
{
	t_str_list	*item;
	int			var_name_len;

	var_name_len = 0;
	while (var[var_name_len] && var[var_name_len] != '=')
		var_name_len++;
	item = ctx->env;
	while (item)
	{
		if (!ft_strncmp(var, item->content, var_name_len)
			&& item->content[var_name_len] == '=')
			return (item);
		item = item->next;
	}
	return (NULL);
}

char	*find_env_value(t_context *ctx, char *var)
{
	t_str_list	*node;

	node = find_env_node(ctx, var);
	if (node)
		return (ft_strchr(node->content, '=') + 1);
	return (NULL);
}

int	del_env(t_context *ctx, char *var_name)
{
	t_str_list	*var;

	if (ft_strchr(var_name, '='))
		return (MS_E_EQINNAME);
	var = find_env_node(ctx, var_name);
	if (!var)
		return (MS_E_ENV_NFOUND);
	ft_str_delitem(&ctx->env, var);
	return (MS_SUCCESS);
}
