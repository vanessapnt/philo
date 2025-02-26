/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:57:57 by varodrig          #+#    #+#             */
/*   Updated: 2025/02/26 17:27:24 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_thinks(t_philo *philos, int time_to_think)
{
	if (must_stop(philos))
		return (1);
	print_state_change(philos, "is thinking");
	if (manage_time(philos, time_to_think) != 0)
	{
		return (1);
	}
	return (0);
}

int	philo_sleeps(t_philo *philos, int time_to_sleep)
{
	if (must_stop(philos))
		return (1);
	print_state_change(philos, "is sleeping");
	if (manage_time(philos, time_to_sleep) != 0)
	{
		return (1);
	}
	return (0);
}

int	must_stop(t_philo *philos)
{
	pthread_mutex_lock(&philos->waiter->dead_mutex);
	if (philos->waiter->time_to_stop == true)
	{
		pthread_mutex_unlock(&philos->waiter->dead_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philos->waiter->dead_mutex);
	return (0);
}

void	*philo_routine(void *arg)
{
	t_philo	*philos;

	philos = (t_philo *)arg;
	if (philos->id % 2 == 0)
		usleep(1000);
	if (philos->waiter->number_of_philosophers == 1)
	{
		pthread_mutex_lock(philos->left_fork);
		print_state_change(philos, "has taken a fork");
		pthread_mutex_unlock(philos->left_fork);
		return (NULL);
	}
	while (!must_stop(philos))
	{
		if (philo_eats(philos, philos->time_to_eat))
			break ;
		if (philo_sleeps(philos, philos->time_to_sleep))
			break ;
		if (philo_thinks(philos, philos->time_to_think))
			break ;
	}
	return (NULL);
}
