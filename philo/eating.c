/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:57:24 by varodrig          #+#    #+#             */
/*   Updated: 2025/02/27 14:00:54 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	lock_forks(t_philo *philos)
{
	if (philos->id % 2 == 1)
	{
		usleep(100);
		pthread_mutex_lock(philos->left_fork);
		print_state_change(philos, "has taken a fork");
		pthread_mutex_lock(philos->right_fork);
		print_state_change(philos, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philos->right_fork);
		print_state_change(philos, "has taken a fork");
		pthread_mutex_lock(philos->left_fork);
		print_state_change(philos, "has taken a fork");
	}
}

void	unlock_forks(t_philo *philos)
{
	usleep(100);
	pthread_mutex_unlock(philos->left_fork);
	pthread_mutex_unlock(philos->right_fork);
}

void	update_meal_status(t_philo *philos)
{
	pthread_mutex_lock(&philos->stomach);
	philos->last_meal = get_current_time();
	philos->eaten_times++;
	if (philos->max_eat != -1 && philos->eaten_times == philos->max_eat)
	{
		pthread_mutex_lock(&philos->waiter->full_mutex);
		philos->waiter->philos_full++;
		pthread_mutex_unlock(&philos->waiter->full_mutex);
	}
	pthread_mutex_unlock(&philos->stomach);
}

int	philo_eats(t_philo *philos, int time_to_eat)
{
	if (must_stop(philos))
		return (1);
	lock_forks(philos);
	pthread_mutex_lock(&philos->waiter->eating_mutex);
	philos->is_eating = true;
	pthread_mutex_unlock(&philos->waiter->eating_mutex);
	update_meal_status(philos);
	print_state_change(philos, "is eating");
	if (manage_time(philos, time_to_eat) || must_stop(philos))
	{
		pthread_mutex_lock(&philos->waiter->eating_mutex);
		philos->is_eating = false;
		pthread_mutex_unlock(&philos->waiter->eating_mutex);
		return (unlock_forks(philos), 1);
	}
	pthread_mutex_lock(&philos->waiter->eating_mutex);
	philos->is_eating = false;
	pthread_mutex_unlock(&philos->waiter->eating_mutex);
	unlock_forks(philos);
	return (0);
}
