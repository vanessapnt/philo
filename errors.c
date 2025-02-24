/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:20:51 by varodrig          #+#    #+#             */
/*   Updated: 2025/02/24 18:20:52 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_err(t_error error)
{
	if (error == NB_PHILO)
		ft_putstr_fd("Too many philosophers. Try with max 200 philosophers\n",
			2);
	else if (error == VALUES_TOO_LOW)
		ft_putstr_fd("The values must be integers and greater than 0.\n", 2);
	else if (error == NB_ARGS)
		ft_putstr_fd("Wrong number of arguments. The program must have 4 or 5 arguments.\n",
			2);
	else if (error == DIGITS_ARGS)
		ft_putstr_fd("The arguments contain invalid characters.\n", 2);
	else if (error == LIMIT_INT_MAX)
		ft_putstr_fd("The values must be lower than INT_MAX.\n", 2);
	exit(EXIT_FAILURE);
}
