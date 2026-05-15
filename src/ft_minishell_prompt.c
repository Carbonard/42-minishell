/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_prompt.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 20:06:16 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/15 22:58:49 by rselva-2         ###   ########.fr       */
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

static void	get_default_prompt(t_context *ctx, char *prompt)
{
	ft_strlcat(prompt, "\e[34m", MAX_PROMPT);
	add_pwd(ctx, prompt);
	ft_strlcat(prompt, "$ ", MAX_PROMPT);
	ft_strlcat(prompt, "\e[0m", MAX_PROMPT);
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

static int	escape_chars(char *ps1, char *prompt)
{
	if (!ft_strncmp(ps1, "\\e", 2))
	{
		ft_strlcat(prompt, "\033", MAX_PROMPT);
		return (2);
	}
	else if (!ft_strncmp(ps1, "\\[", 2))
	{
		ft_strlcat(prompt, "\001", MAX_PROMPT);
		return (2);
	}
	else if (!ft_strncmp(ps1, "\\]", 2))
	{
		ft_strlcat(prompt, "\002", MAX_PROMPT);
		return (2);
	}
	else if (!ft_strncmp(ps1, "\\033", 4))
	{
		ft_strlcat(prompt, "\033", MAX_PROMPT);
		return (4);
	}
	else if (!ft_strncmp(ps1, "\\001", 4))
	{
		ft_strlcat(prompt, "\001", MAX_PROMPT);
		return (4);
	}
	else if (!ft_strncmp(ps1, "\\002", 4))
	{
		ft_strlcat(prompt, "\002", MAX_PROMPT);
		return (4);
	}
	else if (!ft_strncmp(ps1, "\\a", 2))
	{
		ft_strlcat(prompt, "\a", MAX_PROMPT);
		return (2);
	}
	else if (!ft_strncmp(ps1, "\\n", 2))
	{
		ft_strlcat(prompt, "\n", MAX_PROMPT);
		return (2);
	}
	return (0);
}

void	get_prompt(t_context *ctx, char *prompt)
{
	char	*ps1;
	int		i;
	int		escaped_chars;

	prompt[0] = 0;
	ps1 = find_env_value(ctx, "PS1");
	if (!ps1)
	{
		get_default_prompt(ctx, prompt);
		return ;
	}
	i = 0;
	while (ps1[i])
	{
		escaped_chars = 0;
		if (ps1[i] == '\\')
			escaped_chars = escape_chars(ps1 + i, prompt);
		if (escaped_chars)
			i += escaped_chars;
		else if (!ft_strncmp(ps1 + i, "\\w", 2))
			i += add_pwd(ctx, prompt);
		else
			i += add_char(prompt, ps1[i]);
	}
	char *expanded = expand_input(ctx, ft_strdup(prompt));
	ft_strlcpy(prompt, expanded, MAX_PROMPT);
	free(expanded);
}
