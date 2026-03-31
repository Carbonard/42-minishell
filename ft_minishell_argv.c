/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_argv.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 23:34:26 by rselva-2          #+#    #+#             */
/*   Updated: 2026/03/31 20:37:59 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

int	need_to_expand(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			i++;
			while (str[i] != '\'')
				i++;
		}
		// check condition
		else if (str[i] == '$' && str[i + 1] && str[i + 1] != ' ')
			return (i);
		i++;
	}
	return (-1);
}

char	*expand_var(t_context *ctx, char *str, char *expanded, int var_start)
{
	int		var_len;
	char	*aux1;
	char	*aux2;

	var_len = 0;
	while (str[var_start + 1 + var_len]
		&& str[var_start + 1 + var_len] != ' '
		&& str[var_start + 1 + var_len] != '\n'
		&& str[var_start + 1 + var_len] != '"')
		var_len++;
	aux2 = ft_substr(str, var_start + 1, var_len);
	printf("var:'%s'\n", aux2);
	aux1 = ft_strdup(find_env_value(ctx, aux2));
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

char	*expand_cmd(t_context *ctx, char *str)
{
	int		var_start;
	char	*expanded;
	char	*aux1;
	char	*aux2;

	var_start = need_to_expand(str);
	expanded = ft_substr(str, 0, var_start);
	if (str[var_start + 1] == '?')
	{
		aux2 = ft_itoa(ctx->exit_status);
		aux1 = ft_strjoin(expanded, aux2);
		free(aux2);
		free(expanded);
		expanded = ft_strjoin(aux1, str + var_start + 2);
		free(aux1);
	}
	else
		expanded = expand_var(ctx, str, expanded, var_start);
	free(str);
	return (expanded);
}

char	**get_argv_and_redir(t_context *ctx, char *cmd, t_redirection *redir)
{
	char	*command;
	char	**argv;

	command = ft_strdup(cmd);
	while (need_to_expand(command) >= 0)
		command = expand_cmd(ctx, command);
	command = expand_wildcards(command);
	argv = split_cmd(command, redir);
	free(command);
	return (argv);
}
