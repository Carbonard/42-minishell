/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_custom_functions.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 17:01:13 by rselva-2          #+#    #+#             */
/*   Updated: 2026/06/12 01:12:18 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell.h"

int	custom_fork(t_context *ctx)
{
	int	pid;

	pid = fork();
	if (pid >= 0)
		return (pid);
	shell_perror(ctx, "fork");
	ctx->status = MS_E_FORK_FAIL;
	return (-1);
}

int	custom_waitpid(int pid, int *wstatus, int options)
{
	if (pid > 0)
		return (waitpid(pid, wstatus, options));
	if (wstatus)
		*wstatus = 0;
	return (-2);
}

int	custom_dup2(t_context *ctx, int fd1, int fd2)
{
	if (dup2(fd1, fd2) != -1)
		return (0);
	ctx->status = MS_BADFILE;
	shell_perror(ctx, "dup2");
	return (1);
}

int	custom_pipe(t_context *ctx, int fds[2])
{
	if (pipe(fds) != -1)
		return (0);
	ctx->status = MS_BADFILE;
	shell_perror(ctx, "pipe");
	return (1);
}
