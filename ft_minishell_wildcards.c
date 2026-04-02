#include "ft_minishell.h"

// weirdcmp
	// lowalphanum_cmp
		// avanzar hasta alnum
		// comparar lower
	// alphacmp
		// avanzar hasta alnum
		// comparar a pelo
	// strncmp
int	lowalnumcmp(char *s1, char *s2)
{
	int	i1;
	int	i2;

	i1 = 0;
	i2 = 0;
	while(s1[i1] && s2[i2])
	{
		while (!ft_isalnum(s1[i1]) && s1[i1])
			i1++;
		while (!ft_isalnum(s2[i2]) && s2[i2])
			i2++;
		if ((ft_tolower(s2[i2]) - ft_tolower(s1[i1])) > 0)
			return (-1);
		if ((ft_tolower(s2[i2]) - ft_tolower(s1[i1])) < 0)
			return (1);
		i1++;
		i2++;
	}
	return (0);
}
int	alphacmp(char *s1, char *s2)
{
	int i1;
	int	i2;

	i1 = 0;
	i2 = 0;
	while(s1[i1] && s2[i2])
	{
		while (!ft_isalpha(s1[i1]))
			i1++;
		while (!ft_isalpha(s2[i2]))
			i2++;
		if ((s2[i2] - s1[i1]) > 0)
			return (-1);
		if ((s2[i2] - s1[i1]) < 0)
			return (1);
		i1++;
		i2++;
	}
	return (0);
}

int weirdcmp(char *s1, char *s2)
{
	int	cmp;

	cmp = lowalnumcmp(s1, s2);
	if (cmp)
		return (cmp);
	cmp = alphacmp(s1, s2);
	if (cmp)
		return (cmp);
	return (ft_strncmp(s1, s2, ft_strlen(s1)));
}

// find_next_alpha()

void	sort_entries(char **arr)
{
	int		main_i;
	int		secondary_i;
	char	*tmp;
	/*	comparar el 1er string con todos los strings (alphacmp)
		si alguno va antes swichearsi no avanzar
		avanzar el primer comparador al siguiente y repetir
		...
	*/
	main_i = 0;
	while (arr[main_i])
	{
		secondary_i = main_i + 1;	
		while (arr[secondary_i])
		{
			if (weirdcmp(arr[main_i], arr[secondary_i]) > 0)
			{
				tmp = arr[main_i];
				arr[main_i] = arr[secondary_i];
				arr[secondary_i] = tmp;
			}
			secondary_i++;
		}
		main_i++;
	}
	return ;
}

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

/*
	Implement the function 'find_wildcards' to use it in the function below.
	Help: You can use the 'advance_quotes' function implemented on 'ft_minishell_split_cmd.c' file
*/
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
		entries.length = 0;
		free(entries.arr);
		i = find_wildcards(cmd);
	}
	return (cmd);
}
