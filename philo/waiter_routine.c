/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waiter_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:58:18 by varodrig          #+#    #+#             */
/*   Updated: 2025/02/26 20:03:10 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_starved(t_philo *philos, long long time_to_die)
{
	long long	time_since_last_meal;

	pthread_mutex_lock(&philos->stomach);
	time_since_last_meal = get_current_time() - philos->last_meal;
	pthread_mutex_unlock(&philos->stomach);
	return (time_since_last_meal >= time_to_die);
}

int	is_eating(t_philo *philos)
{
	int	eating_status;

	pthread_mutex_lock(&philos->waiter->eating_mutex);
	eating_status = philos->is_eating;
	pthread_mutex_unlock(&philos->waiter->eating_mutex);
	return (eating_status);
}

int	philo_dead(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos->waiter->number_of_philosophers)
	{
		if (philo_starved(&philos[i], philos[i].time_to_die)
			&& is_eating(&philos[i]) == 0)
		{
			print_state_change(&philos[i], "died");
			pthread_mutex_lock(&philos->waiter->dead_mutex);
			philos->dead = true;
			pthread_mutex_unlock(&philos->waiter->dead_mutex);
			return (1);
		}
		i++;
	}
	return (0);
}

int	everyone_full(t_philo *philos)
{
	pthread_mutex_lock(&philos->waiter->full_mutex);
	if (philos->waiter->philos_full == philos->waiter->number_of_philosophers)
	{
		pthread_mutex_unlock(&philos->waiter->full_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philos->waiter->full_mutex);
	return (0);
}

void	*waiter_routine(void *arg)
{
	t_philo	*philos;

	philos = (t_philo *)arg;
	while (1)
	{
		if (philo_dead(philos) || everyone_full(philos))
		{
			pthread_mutex_lock(&philos->waiter->dead_mutex);
			philos->waiter->time_to_stop = true;
			pthread_mutex_unlock(&philos->waiter->dead_mutex);
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}
