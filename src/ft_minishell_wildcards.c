/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_wildcards.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 15:47:56 by rselva-2          #+#    #+#             */
/*   Updated: 2026/04/19 00:13:33 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_execution.h"

static char	*expand_wc(char *cmd, char *start, t_dyn_ptr *entries)
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
	ft_strlcpy(new_cmd, cmd, start - cmd + 1);
	i = 0;
	while (i < entries->length)
	{
		ft_strlcat(new_cmd, entries->arr[i], length);
		if (i < entries->length - 1)
			ft_strlcat(new_cmd, " ", length);
		i++;
	}
	ft_strlcat(new_cmd, ft_strchr(start, ' '), length);
	free(cmd);
	return (new_cmd);
}

static int	check_entry(char* ent_name, char *pattern, char**match)
{
	int	i;

	i = 0;
	if (pattern[0] != '*')
	{
		if (ft_strncmp(match[0], ent_name, ft_strlen(match[0])))
			return (0);
		i++;
		ent_name += ft_strlen(match[i]);
	}
	while (match[i])
	{
		if (!ft_strnstr(ent_name, match[i], ft_strlen(ent_name)))
			return (0);
		ent_name = ft_strnstr(ent_name, match[i], ft_strlen(ent_name));
		ent_name += ft_strlen(match[i]);
		i++;
	}
	if (!(pattern[ft_strlen(pattern) - 1] != '*' && ft_strncmp(
		ent_name + ft_strlen(ent_name) - ft_strlen(match[i - 1]), match[i - 1],
		ft_strlen(match[i - 1]))))
			return (1);
	return (0);
}

static int	save_coincidences(t_dyn_ptr *entries, char *pattern)
{
	char			wd[PATH_MAX];
	DIR				*dir;
	struct dirent	*entry;
	char			**match;

	dir = opendir(getcwd(wd, PATH_MAX));
	entry = readdir(dir);
	match = ft_split(pattern, '*');
	while (entry)
	{
		if (entry->d_name[0] != '.'
			&& check_entry(entry->d_name, pattern, match))
			add_ptr(entries, ft_strdup(entry->d_name));
		entry = readdir(dir);
	}
	free_split(match);
	closedir(dir);
	return (entries->length);
}

static char	*expand_wc_wrapper(char *cmd, char *start, char *end)
{
	char		*word;
	t_dyn_ptr	entries;

	if (end)
		word = ft_substr(start, 0, end - start);
	else
		word = ft_strdup(start);
	init_dyn_ptr(&entries, 0);
	if (ft_strchr(word, '*'))
	{
		if (save_coincidences(&entries, word))
			cmd = expand_wc(cmd, start, &entries);
	}
	free(word);
	free_dyn_ptr(&entries);
	return (cmd);
}

char	*expand_wildcards(char *cmd)
{
	char		*start;
	char		*end;
	int			index;

	start = cmd;
	index = 0;
	while (start && *start)
	{
		end = ft_strchr(start + 1, ' ');
		cmd = expand_wc_wrapper(cmd, start, end);
		if (end)
		{
			index += (end - start);
			start = cmd + index;
			while (*start == ' ')
				start++;
		}
		else
			start = NULL;
	}
	return (cmd);
}
