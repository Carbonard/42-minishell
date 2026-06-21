/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_expand_variable.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/06 20:44:24 by rselva-2          #+#    #+#             */
/*   Updated: 2026/06/21 16:25:01 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_execution.h"

static int	var_lenght(char *str, int var_start)
{
	int	var_len;

	var_len = 0;
	while (str[var_start + 1 + var_len]
		&& (ft_isalnum(str[var_start + 1 + var_len])
			|| str[var_start + 1 + var_len] == '_'))
		var_len++;
	return (var_len);
}

char	*triple_join(char *first, char *second, char *third)
{
	char	*ret;
	char	*aux;

	aux = ft_strjoin(first, second);
	if (!aux)
		return (NULL);
	ret = ft_strjoin(aux, third);
	free(aux);
	return (ret);
}

static char	*expand_env_var(t_context *ctx, char *str, int var_start, int *i)
{
	char	*before_var;
	char	*var_name;
	char	*var_value;
	char	*ending;
	char	*expanded;

	var_name = ft_substr(str, var_start + 1, var_lenght(str, var_start));
	if (!var_name)
		return (NULL);
	var_value = find_env_value(ctx, var_name);
	free(var_name);
	*i += ft_strlen(var_value);
	before_var = ft_substr(str, 0, var_start);
	ending = ft_strdup(str + var_start + var_lenght(str, var_start) + 1);
	if (!ending || !before_var)
	{
		free(ending);
		free(before_var);
		return (NULL);
	}
	expanded = triple_join(before_var, var_value, ending);
	free(ending);
	free(before_var);
	return (expanded);
}

static char	*expand_e_status(t_context *ctx, char *str, int var_start, int *i)
{
	char	*expanded;
	char	*aux_expanded;
	char	*status_str;

	expanded = ft_substr(str, 0, var_start);
	if (!expanded)
		return (NULL);
	status_str = ft_itoa(ctx->exit_status);
	if (!status_str)
	{
		free(expanded);
		return (NULL);
	}
	*i += ft_strlen(status_str);
	aux_expanded = ft_strjoin(expanded, status_str);
	free(status_str);
	free(expanded);
	if (!aux_expanded)
		return (NULL);
	expanded = ft_strjoin(aux_expanded, str + var_start + 2);
	free(aux_expanded);
	return (expanded);
}

char	*expand_variable(t_context *ctx, char *str, int *i)
{
	int		var_start;
	char	*expanded;

	var_start = *i;
	expanded = NULL;
	if (str[var_start + 1] == '?')
		expanded = expand_e_status(ctx, str, var_start, i);
	else if (!ft_strchr(INVALID_EXPORT_CHARS, str[var_start + 1]))
		expanded = expand_env_var(ctx, str, var_start, i);
	free(str);
	return (expanded);
}
