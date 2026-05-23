/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_execute_leaf.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 03:50:34 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/23 03:31:46 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_execution.h"
#include "ft_minishell_input.h"

void mock_free(void *ptr)
{
	(void)ptr;
}

static t_str_list	*init_argv(t_context *ctx, t_dyn_ptr *argv, t_command_tree *node, t_str_list **tokens)
{
	char		*expanded_cmd;

	ctx->original_in = dup(STDIN_FILENO);
	ctx->original_out = dup(STDOUT_FILENO);
	expanded_cmd = expand_input(ctx, ft_strdup(node->cmd));
	init_dyn_ptr(argv, 1);
	*tokens = get_tokens(expanded_cmd);
	free(expanded_cmd);
	return (*tokens);
}

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
		if (ft_strncmp(name + ft_strlen(name) - ft_strlen(pattern[i - 1]), pattern[i - 1], ft_strlen(pattern[i - 1])))
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
		// else
		// 	printf("No match: '%s'\n", entry->d_name);
		entry = readdir(dir);
	}
	closedir(dir);
	free_split(pattern);
	return (matches.arr);
}

int	expand_wildcard(char *token, t_dyn_ptr *argv)
{
	char		**matches;
	int			i_match;
	int			ret;

	ret = 0;
	matches = find_matches(token);
	if (!matches)
		return (0);
	sort_entries(matches);
	i_match = 0;
	while (matches[i_match])
	{
		add_ptr(argv, matches[i_match]);
		i_match++;
		ret = 1;
	}
	free(matches);
	return (ret);
}

static void	refine_token(t_dyn_ptr *argv, t_str_list *token)
{
	t_str_list	*new_tokens;
	t_str_list	*token_i;

	new_tokens = get_tokens(token->content);
	token_i = new_tokens;
	while (token_i)
	{
		if (!expand_wildcard(token_i->content, argv))
			add_ptr(argv, remove_quotes	(token_i->content));
		else
			free(token_i->content);
		token_i = token_i->next;
	}
	ft_str_lstclear(&new_tokens, mock_free);
}

// static void	refine_token(t_dyn_ptr *argv, t_str_list *token)
// {
// 	t_str_list	*new_tokens;
// 	t_str_list	*token_i;

// 	token->content = expand_wildcards(token->content);
// 	new_tokens = get_tokens(token->content);
// 	token_i = new_tokens;
// 	while (token_i)
// 	{
// 		remove_quotes(token_i->content);
// 		add_ptr(argv, token_i->content);
// 		token_i = token_i->next;
// 	}
// 	ft_str_lstclear(&new_tokens, mock_free);
// }

char	**manage_argv_and_redirections(t_context *ctx, t_command_tree *node)
{
	t_str_list	*tokens;
	t_str_list	*token;
	t_dyn_ptr	argv;

	token = init_argv(ctx, &argv, node, &tokens);
	while (token)
	{
		if ((token->content[0] == '<' || token->content[0] == '>') && token->next)
		{
			if (!manage_redirection(ctx, token, node->here_doc))
			{
				ft_str_lstclear(&tokens, free);
				free_split(argv.arr);
				return (NULL);
			}
			token = token->next;
		}
		else
			refine_token(&argv, token);
		token = token->next;
	}
	ft_str_lstclear(&tokens, free);
	return (argv.arr);
}

int	execute_leaf(t_context *ctx, t_command_tree *node)
{
	int		pid;
	char	**cmd_argv;

	cmd_argv = manage_argv_and_redirections(ctx, node);
	ctx->read_exit_status = 1;
	if (!cmd_argv)
	{
		restore_redirections(ctx);
		return (0);
	}
	if (try_builtins(ctx, cmd_argv))
		return (0);
	ctx->read_exit_status = 0;
	pid = fork();
	if (pid == 0)
	{
		close_redirections(ctx);
		if (ctx->status == MS_SUCCESS)
			execute_command(ctx, cmd_argv);
		else
			free_split(cmd_argv);
		silent_exit(ctx, ctx->exit_status);
	}
	restore_redirections(ctx);
	signal(SIGINT, SIG_IGN);
	free_split(cmd_argv);
	return (pid);
}
