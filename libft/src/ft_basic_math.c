/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_basic_math.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 17:03:39 by rselva-2          #+#    #+#             */
/*   Updated: 2025/11/24 15:47:03 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_abs(int n)
{
	if (n < 0)
		return (-n);
	return (n);
}

float	ft_fabs(float n)
{
	if (n < 0)
		return (-n);
	return (n);
}

int	ft_max(int n, int m)
{
	if (m > n)
		return (m);
	return (n);
}

int	ft_min(int n, int m)
{
	if (m > n)
		return (n);
	return (m);
}

int	ft_sign(int n)
{
	if (n > 0)
		return (1);
	else if (n < 0)
		return (-1);
	return (0);
}
