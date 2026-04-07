/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_wildcards.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 15:47:56 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/03 18:59:25 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_execution.h"

char	*expand_wc(char *cmd, int wc_i, t_dyn_ptr *entries)
{
	int		length;
	size_t	i;
	char	*new_cmd;

	sort_entries(entries->arr);
	length = ft_strlen(cmd) + 1;
	i = 0;
	while (i < entries->length)
	{
		length += ft_strlen(entries->arr[i]) + 1;
		i++;
	}
	new_cmd = malloc(length);
	ft_strlcpy(new_cmd, cmd, wc_i + 1);
	i = 0;
	while (i < entries->length - 1)
	{
		if (!(wc_i == 0 && i == 0))
			ft_strlcat(new_cmd, " ", length);
		ft_strlcat(new_cmd, entries->arr[i], length);
		i++;
	}
	ft_strlcat(new_cmd, cmd + wc_i + 1, length);
	free(cmd);
	return (new_cmd);
}

int	find_wildcards(char *cmd)
{
	int	i;

	i = 1;
	while (cmd[i])
	{
		if (cmd[i] == '*' && cmd[i - 1] == ' '
			&& (cmd[i + 1] == ' ' || cmd[i + 1] == '\n' || !cmd[i + 1]))
			return (i);
		// do smth if there's a file named * or whtevr
		i += advance_quotes(cmd + i);
	}
	return (0);
}

char	*expand_wildcards(char *cmd)
{
	int				i;
	char			wd[PATH_MAX];
	DIR				*dir;
	struct dirent	*entry;
	t_dyn_ptr		entries;

	i = find_wildcards(cmd);
	while (i)
	{
		init_dyn_ptr(&entries, 1);
		dir = opendir(getcwd(wd, PATH_MAX));
		entry = readdir(dir);
		while (entry)
		{
			if (entry->d_name[0] != '.')
				add_ptr(&entries, entry->d_name);
			entry = readdir(dir);
		}
		add_ptr(&entries, NULL);
		cmd = expand_wc(cmd, i, &entries);
		closedir(dir);
		free_dyn_ptr(&entries);
		i = find_wildcards(cmd);
	}
	return (cmd);
}
