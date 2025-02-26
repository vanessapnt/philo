/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:57:52 by varodrig          #+#    #+#             */
/*   Updated: 2025/02/26 17:16:00 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(char c)
{
	return (c >= '0' && c <= '9');
}

int	ft_isblank(int c)
{
	if ((c > 8 && c < 14) || (c == 32))
		return (1);
	return (0);
}

static int	ft_is_negative(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '-')
			return (1);
		i++;
	}
	return (0);
}

static int	ft_only_digits(char *str)
{
	int	i;

	if (!str || *str == '\0')
		return (1);
	i = 0;
	while (ft_isblank(str[i]) == 1)
		i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (1);
		i++;
	}
	return (0);
}

void	parsing(int argc, char **argv)
{
	int	i;

	if (argc != 5 && argc != 6)
		ft_err(NB_ARGS);
	i = 0;
	while (argv[++i])
	{
		if (ft_only_digits(argv[i]))
			ft_err(DIGITS_ARGS);
	}
	i = 0;
	while (argv[++i])
	{
		if (ft_is_negative(argv[i]))
			ft_err(VALUES_TOO_LOW);
		if (ft_atol(argv[i]) > INT_MAX)
			ft_err(LIMIT_INT_MAX);
	}
	if (ft_atoi(argv[1]) > 200)
		ft_err(NB_PHILO);
	if (ft_atoi(argv[1]) == 0)
		ft_err(ZERO_PHILO);
}
