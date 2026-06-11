/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_children.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 17:01:13 by rselva-2          #+#    #+#             */
/*   Updated: 2026/06/08 22:01:40 by rselva-2         ###   ########.fr       */
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
	shell_perror(ctx, NULL);
	return (1);
}
