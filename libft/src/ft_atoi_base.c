/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rselva-2 <rselva-2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 22:54:25 by rselva-2          #+#    #+#             */
/*   Updated: 2025/11/29 22:52:35 by rselva-2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	base_length(char *base)
{
	int	len;
	int	i;

	len = 0;
	while (base[len] != '\0')
	{
		if (base[len] == '+' || base[len] == '-' || base[len] == ' '
			|| base[len] == '\n' || base[len] == '\t' || base[len] == '\r'
			|| base[len] == '\f' || base[len] == '\v')
			return (-1);
		len ++;
	}
	while (*base)
	{
		i = 0;
		while (base[i] && base[i + 1])
		{
			if (*base == base[i + 1])
				return (-1);
			i++;
		}
		base++;
	}
	return (len);
}

static int	sign(char **str)
{
	int	sgn;

	sgn = 1;
	while (**str == ' ' || **str == '\n' || **str == '\t' || **str == '\v'
		|| **str == '\f' || **str == '\r')
		(*str)++;
	while (**str == '-' || **str == '+')
	{
		if (**str == '-')
			sgn = -sgn;
		(*str)++;
	}
	return (sgn);
}

int	index_in_str(char c, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	if (c == 0)
		return (i);
	return (-1);
}

int	ft_atoi_base(char *str, char *base)
{
	int	base_len;
	int	number;
	int	sgn;

	if (!str || !base)
		return (0);
	base_len = base_length(base);
	if (base_len <= 1)
		return (0);
	sgn = sign(&str);
	number = 0;
	while (*str && index_in_str(*str, base) >= 0)
	{
		number = number * base_len + sgn * index_in_str(*str, base);
		str++;
	}
	return (number);
}
