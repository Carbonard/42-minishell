/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_argv.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 23:34:26 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/17 01:48:39 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_execution.h"

int	need_to_expand(char *str, int i)
{
	while (str[i])
	{
		if (str[i] == '\'')
		{
			i++;
			while (str[i] != '\'')
				i++;
		}
		// check condition
		else if (str[i] == '$' && (ft_isalpha(str[i + 1]) || str[i+1] == '_'
				|| str[i+1] == '?'))
			return (i);
		i++;
	}
	return (-1);
}

char	*expand_var(t_context *ctx, char *str, int var_start, int *i)
{
	int		var_len;
	char	*aux1;
	char	*aux2;
	char	*expanded;

	expanded = ft_substr(str, 0, var_start);
	var_len = 0;
	while (str[var_start + 1 + var_len]
		&& (ft_isalnum(str[var_start + 1 + var_len])
			|| str[var_start + 1 + var_len] == '_'))
		var_len++;
	aux2 = ft_substr(str, var_start + 1, var_len);
	aux1 = ft_strdup(find_env_value(ctx, aux2));
	*i += ft_strlen(aux1) - ft_strlen(aux2);
	free(aux2);
	aux2 = ft_strjoin(expanded, aux1);
	free(expanded);
	free(aux1);
	aux1 = ft_strdup(str + var_start + var_len + 1);
	expanded = ft_strjoin(aux2, aux1);
	free(aux2);
	free(aux1);
	return (expanded);
}

char	*expand_cmd(t_context *ctx, char *str, int *i)
{
	int		var_start;
	char	*expanded;
	char	*aux1;
	char	*aux2;

	var_start = need_to_expand(str, *i);
	if (str[var_start + 1] == '?')
	{
		expanded = ft_substr(str, 0, var_start);
		aux2 = ft_itoa(ctx->exit_status);
		*i += -1 + ft_strlen(aux2);
		aux1 = ft_strjoin(expanded, aux2);
		free(aux2);
		free(expanded);
		expanded = ft_strjoin(aux1, str + var_start + 2);
		free(aux1);
	}
	else if (!ft_strchr(INVALID_EXPORT_CHARS, str[var_start + 1])){
		expanded = expand_var(ctx, str, var_start, i);}
	free(str);
	return (expanded);
}

char	**get_argv_and_redir(t_context *ctx, char *cmd, t_redirection *redir)
{
	char	*command;
	char	**argv;
	int		i;

	command = ft_strdup(cmd);
	i = need_to_expand(command, 0);
	while (i > 0)
	{
		command = expand_cmd(ctx, command, &i);
		i++;
		i = need_to_expand(command, i);
	}
	command = expand_wildcards(command);
	argv = split_cmd(command, redir);
	free(command);
	return (argv);
}
