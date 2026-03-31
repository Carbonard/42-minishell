#include "ft_minishell.h"

void	sort_entries(t_dyn_ptr *entries)
{
	(void)entries;
}

char	*expand_wc(char *cmd, int wc_i, t_dyn_ptr *entries)
{
	int		length;
	size_t	i;
	char	*new_cmd;

	sort_entries(entries);
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
	while (i < entries->length)
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

/*
	Implement the function 'find_wildcards' to use it in the function below.
	Help: You can use the 'advance_quotes' function implemented on 'ft_minishell_split_cmd.c' file
*/

char	*expand_wildcards(char *cmd)
{
	int				i;
	char			wd[PATH_MAX];
	DIR				*dir;
	struct dirent	*entry;
	t_dyn_ptr		entries;

	i = 0;
	while (i)
	{
		init_dyn_ptr(&entries, 1);
		dir = opendir(getcwd(wd, PATH_MAX));
		entry = readdir(dir);
		while (entry)
		{
			add_ptr(&entries, entry->d_name);
			entry = readdir(dir);
		}
		closedir(dir);
		cmd = expand_wc(cmd, i, &entries);
		entries.length = 0;
		free(entries.arr);
	}
	return (cmd);
}
