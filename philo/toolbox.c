/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   toolbox.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:20:51 by varodrig          #+#    #+#             */
/*   Updated: 2025/02/26 17:17:28 by varodrig         ###   ########.fr       */
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
	{
		ft_putstr_fd("Wrong number of arguments.\n", 2);
		ft_putstr_fd("The program must have 4 or 5 arguments.\n", 2);
	}
	else if (error == DIGITS_ARGS)
		ft_putstr_fd("The arguments contain invalid characters.\n", 2);
	else if (error == LIMIT_INT_MAX)
		ft_putstr_fd("The values must be lower than INT_MAX.\n", 2);
	else if (error == ZERO_PHILO)
		ft_putstr_fd("There should be someone at the party.\n", 2);
	exit(EXIT_FAILURE);
}

/*
struct timeval {
	time_t      tv_sec;  // Seconds since January 1, 1970
	suseconds_t tv_usec; // Microseconds (0 to 999999)
};
*/
long long	get_current_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return ((tv.tv_sec) * (long long)1000 + (tv.tv_usec) / 1000);
}

void	print_state_change(t_philo *philos, char *str)
{
	long long	time;

	pthread_mutex_lock(&philos->waiter->write_mutex);
	if (!must_stop(philos))
	{
		time = get_current_time() - philos->start_time;
		printf("%lld %d %s\n", time, philos->id, str);
	}
	pthread_mutex_unlock(&philos->waiter->write_mutex);
}

int	manage_time(t_philo *philos, int time_to_wait)
{
	long long	start_time;
	long long	elapsed_time;

	start_time = get_current_time();
	while (1)
	{
		usleep(50);
		elapsed_time = get_current_time() - start_time;
		if (elapsed_time >= time_to_wait)
			break ;
		if (must_stop(philos))
		{
			return (1);
		}
	}
	usleep(500);
	return (0);
}
