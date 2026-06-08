/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_wildcards.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 15:47:56 by rselva-2          #+#    #+#             */
/*   Updated: 2026/06/07 16:18:52 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_execution.h"

char	**find_pattern(char *token)
{
	int			start;
	int			end;
	t_dyn_ptr	matches;

	start = 0;
	end = 0;
	init_dyn_ptr(&matches, 1);
	while (token[end])
	{
		while (token[end] && token[end] != '*')
			end += advance_quotes(token + end);
		if (end == start)
		{
			add_ptr(&matches, ft_calloc(1, 1));
			end++;
		}
		else
			add_ptr(&matches,
				remove_quotes(ft_substr(token, start, end - start)));
		start = end;
	}
	return (matches.arr);
}

int	pattern_matches(char **pattern, char *name)
{
	int		i;

	i = 0;
	if (name != ft_strnstr(name, pattern[0], ft_strlen(name)))
		return (0);
	while (pattern[i])
	{
		name = ft_strnstr(name, pattern[i], ft_strlen(name));
		if (!name)
			return (0);
		name += ft_strlen(pattern[i]);
		i++;
	}
	if (i && !pattern[i] && *name && pattern[i - 1][0])
	{
		name -= ft_strlen(pattern[i - 1]);
		if (ft_strncmp(name + ft_strlen(name) - ft_strlen(pattern[i - 1]),
				pattern[i - 1], ft_strlen(pattern[i - 1])))
			return (0);
	}
	return (1);
}

char	**find_matches(char *token)
{
	char			wd[PATH_MAX];
	DIR				*dir;
	struct dirent	*entry;
	t_dyn_ptr		matches;
	char			**pattern;

	pattern = find_pattern(token);
	if (!getcwd(wd, PATH_MAX))
	{
		free_split(pattern);
		return (NULL);
	}
	dir = opendir(wd);
	entry = readdir(dir);
	init_dyn_ptr(&matches, 1);
	while (entry)
	{
		if (entry->d_name[0] != '.' && pattern_matches(pattern, entry->d_name))
			add_ptr(&matches, ft_strdup(entry->d_name));
		entry = readdir(dir);
	}
	closedir(dir);
	free_split(pattern);
	return (matches.arr);
}

static void	free_remaining(char **matches, int i_match)
{
	while (matches[i_match])
	{
		free(matches[i_match]);
		i_match++;
	}
	free(matches);
}

/*
	This function returns:
		 0 if anything done
		 1 if wildcards are expanded
		-1 on error
*/
int	expand_wildcards(char *token, t_dyn_ptr *argv)
{
	char		**matches;
	int			i_match;
	int			ret;

	ret = 0;
	if (!ft_strchr(token, '*'))
		return (0);
	matches = find_matches(token);
	if (!matches)
		return (0);
	sort_entries(matches);
	i_match = 0;
	while (matches[i_match])
	{
		if (!add_ptr(argv, matches[i_match]))
		{
			free_remaining(matches, i_match);
			return (-1);
		}
		i_match++;
		ret = 1;
	}
	free(matches);
	return (ret);
}
