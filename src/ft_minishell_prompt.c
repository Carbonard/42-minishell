/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_prompt.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 20:06:16 by rselva-2          #+#    #+#             */
/*   Updated: 2026/06/06 20:27:37 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_input.h"
#include "ft_minishell_execution.h"

static int	add_pwd(t_context *ctx, char *prompt)
{
	char	home[PATH_MAX];
	char	pwd[PATH_MAX];
	char	*saved_pwd;

	ft_strlcpy(home, find_env_value(ctx, "HOME"), 100);
	saved_pwd = find_env_value(ctx, "PWD");
	if (saved_pwd)
		ft_strlcpy(pwd, saved_pwd, PATH_MAX);
	else if (!getcwd(pwd, PATH_MAX))
		ft_strlcpy(pwd, ".", PATH_MAX);
	if (home[0] && !ft_strncmp(home, pwd, ft_strlen(home)))
	{
		ft_strlcat(prompt, "~", MAX_PROMPT);
		ft_strlcat(prompt, pwd + ft_strlen(home), MAX_PROMPT);
	}
	else
		ft_strlcat(prompt, pwd, MAX_PROMPT);
	return (2);
}

static int	add_char(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && i < MAX_PROMPT - 1)
		i++;
	if (i < MAX_PROMPT - 1)
	{
		str[i] = c;
		i++;
	}
	str[i] = 0;
	return (1);
}

void	get_prompt(t_context *ctx, char *prompt)
{
	char	*ps1;
	int		i;
	char	*expanded;

	ps1 = find_env_value(ctx, "PS1");
	if (!ps1)
	{
		prompt[0] = 0;
		return ;
	}
	expanded = expand_variables(ctx, ft_strdup(ps1));
	prompt[0] = 0;
	i = 0;
	while (expanded[i])
	{
		if (!ft_strncmp(expanded + i, "\\w", 2))
			i += add_pwd(ctx, prompt);
		else
			i += add_char(prompt, expanded[i]);
	}
	str_replace(prompt, "\\$", "$");
	free(expanded);
	escape_text(prompt);
}
