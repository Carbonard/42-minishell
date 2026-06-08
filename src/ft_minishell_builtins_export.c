/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_builtins_export.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 15:51:24 by elangari          #+#    #+#             */
/*   Updated: 2026/06/07 21:25:58 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_builtins.h"

static void	order_export(t_dyn_ptr *exp)
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
			if (ft_strncmp(
					exp->arr[i], exp->arr[si], ft_strlen(exp->arr[i])) > 0)
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
}

static int	print_export(t_context *ctx)
{
	t_dyn_ptr	exp;
	t_str_list	*env;
	size_t		i;

	i = 0;
	env = ctx->env;
	init_dyn_ptr(&exp, 1);
	while (env)
	{
		if (!add_ptr(&exp, env->content))
		{
			free(exp.arr);
			return (MS_E_MALLOC);
		}
		env = env->next;
	}
	order_export(&exp);
	while (i < exp.length)
	{
		printf("export %s\n", exp.arr[i]);
		i++;
	}
	free(exp.arr);
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
			return (MS_E_MALLOC);
		return (MS_SUCCESS);
	}
	return (add_env(ctx, var));
}

int	builtin_export(t_context *ctx, char **argv)
{
	int	status;
	int	arg_i;

	if (!argv[1])
	{
		if (print_export(ctx) == MS_E_MALLOC)
			return (MS_E_MALLOC);
		return (MS_SUCCESS);
	}
	arg_i = 1;
	while (argv[arg_i])
	{
		status = export(ctx, argv[arg_i]);
		if (status == MS_NOT_VALID_ID)
		{
			ctx->status = MS_NOT_VALID_ID;
			shell_arg_error(ctx, argv, arg_i);
			ctx->exit_status = 2;
		}
		else if (status == MS_E_MALLOC)
			return (status);
		arg_i++;
	}
	return (ctx->status);
}
