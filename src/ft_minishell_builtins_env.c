/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_builtins_env.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 18:55:11 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/18 22:32:15 by rselva-2         ###   ########.fr       */
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

static int	invalid_identifier(char *var)
{
	int		i;

	if (!ft_isalpha(var[0]) && var[0] != '_')
		return (1);
	i = 0;
	while (var[i] && var[i] != '=')
	{
		if (!isalnum(var[i]) && var[i] != '_')
			return (1);
		i++;
	}
	return (0);
}

int	export(t_context *ctx, char *var)
{
	t_str_list	*item;

	if (invalid_identifier(var))
		return (MS_NOT_VALID_ID);
	if (!ft_strchr(var, '='))
		return (MS_SUCCESS);
	item = find_env_node(ctx, var);
	if (item)
	{
		free(item->content);
		item->content = ft_strdup(var);
		if (!item->content)
			return (MS_E_ENV_MALLOC);
		return (MS_SUCCESS);
	}
	return (add_env(ctx, var));
}

int	builtin_export(t_context *ctx, char **argv)
{
	int status;
	int	final_status;

	final_status = MS_SUCCESS;
	while (*argv)
	{
		status = export(ctx, *argv);
		if (status == MS_NOT_VALID_ID)
			final_status = status;
		else if (status == MS_E_ENV_MALLOC)
			return (status);
		argv++;
	}
	return (final_status);
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
		if (ft_strchr(var, '='))
			return (MS_SUCCESS);
		item = find_env_node(ctx, var);
		if (item)
			ft_str_delitem(&ctx->env, item);
		arg_i++;
	}
	return (MS_SUCCESS);
}
