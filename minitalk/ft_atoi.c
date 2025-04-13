/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessaber <aessaber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:57:30 by aessaber          #+#    #+#             */
/*   Updated: 2025/04/13 18:42:28 by aessaber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static int	ft_isspace(int c)
{
	return ((c >= '\t' && c <= '\r') || c == ' ');
}

static int	ft_issign(char c)
{
	return (c == '+' || c == '-');
}

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

static int	ft_is_overflowed(size_t num, char c)
{
	return (num > __LONG_MAX__ / 10
		|| (num == __LONG_MAX__ / 10 && (c - '0') > 7));
}

int	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	size_t	num;

	i = 0;
	while (ft_isspace(str[i]))
		i++;
	sign = 1;
	if (ft_issign(str[i]))
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	num = 0;
	while (ft_isdigit(str[i]))
	{
		if (ft_is_overflowed(num, str[i]))
			return (-1 * (sign == 1));
		num = num * 10 + (str[i] - '0');
		i++;
	}
	return ((num * sign));
}
