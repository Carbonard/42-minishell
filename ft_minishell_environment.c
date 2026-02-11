/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_environment.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 22:18:08 by rselva-2          #+#    #+#             */
/*   Updated: 2026/02/11 14:08:52 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

void	set_shell(t_context *ctx, char *shell_name)
{
	char	*shell;
	char	*var;
	int		last_slash;

	if (shell_name[0] != '.' && shell_name[0] != '/' && shell_name[0] != '~')
	{
		ctx->shell_name = shell_name;
		shell = find_cmd_path(ctx, shell_name);
	}
	else
	{
		shell = malloc(MAX_PWD + 20);
		last_slash = ft_strlen(shell_name);
		while (shell_name[last_slash] != '/')
			last_slash--;
		shell_name[last_slash] = 0;
		cd(ctx, shell_name);
		// ft_strlcpy(shell, "SHELL=", MAX_PWD + 20);
		getcwd(shell, MAX_PWD);
		ft_strlcat(shell, "/", MAX_PWD + 20);
		ft_strlcat(shell, shell_name + last_slash + 1, MAX_PWD + 20);
		ctx->shell_name = shell_name + last_slash + 1;
		cd(ctx, "-");
		unset(ctx, "OLDPWD");
	}
	var = ft_strjoin("SHELL=", shell);
	export(ctx, var);
	free(var);
	free(shell);
}

int	save_env(t_context *ctx, char **original_env)
{
	ctx->env = NULL;
	while (*original_env)
	{
		if (add_env(ctx, *original_env) != MS_SUCCESS)
			return (MS_E_ENV_MALLOC);
		original_env++;
	}
	return (MS_SUCCESS);
}

int	add_env(t_context *ctx, char *new_var)
{
	t_str_list	*new_item;

	new_var = ft_strdup(new_var);
	if (!new_var)
		return (MS_E_ENV_MALLOC);
	new_item = ft_str_lstnew(new_var);
	if (!new_item)
		return (MS_E_ENV_MALLOC);
		//Borrar todo?
	ft_str_lstadd_back(&ctx->env, new_item);
	return (MS_SUCCESS);
}

// This function can receive VAR=value or just VAR
t_str_list	*find_env_node(t_context *ctx, char *var)
{
	t_str_list	*item;
	int			var_name_len;

	var_name_len = 0;
	while (var[var_name_len] && var[var_name_len] != '=')
		var_name_len++;
	item = ctx->env;
	while (item)
	{
		if (!ft_strncmp(var, item->content, var_name_len)
			&& item->content[var_name_len] == '=')
			return (item);
		item = item->next;
	}
	return (NULL);
}

char	*find_env_value(t_context *ctx, char *var)
{
	t_str_list	*node;

	node = find_env_node(ctx, var);
	if (node)
		return (ft_strchr(node->content, '=') + 1);
	return (NULL);
}

int	del_env(t_context *ctx, char *var_name)
{
	t_str_list	*var;

	if (ft_strchr(var_name, '='))
		return (MS_E_EQINNAME);
	var = find_env_node(ctx, var_name);
	if (!var)
		return (MS_E_ENV_NFOUND);
	ft_str_delitem(&ctx->env, var);
	return (MS_SUCCESS);
}
