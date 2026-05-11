/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_init_config2.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 12:55:41 by rselva-2          #+#    #+#             */
/*   Updated: 2026/05/11 13:10:11 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minishell_builtins.h"

void	increment_shlvl(t_context *ctx)
{
	char	*inherited_var;
	char	new_var[17];
	int		old_shlvl;
	char	*new_shlvl;

	inherited_var = find_env_value(ctx, "SHLVL");
	if (!inherited_var)
	{
		export(ctx, "SHLVL=1");
		return ;
	}
	old_shlvl = ft_atoi(inherited_var);
	new_shlvl = ft_itoa(old_shlvl + 1);
	ft_strlcpy(new_var, "SHLVL=", 17);
	if (new_shlvl)
		ft_strlcat(new_var, new_shlvl, 17);
	else
		ft_strlcat(new_var, inherited_var, 17);
	export(ctx, new_var);
	free(new_shlvl);
}
