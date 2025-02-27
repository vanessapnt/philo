/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:57:38 by varodrig          #+#    #+#             */
/*   Updated: 2025/02/27 12:43:56 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_parameters(int i, char **argv, t_philo *philos)
{
	philos[i].time_to_die = ft_atoi(argv[2]);
	philos[i].time_to_eat = ft_atoi(argv[3]);
	philos[i].time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		philos[i].max_eat = ft_atoi(argv[5]);
	else
		philos[i].max_eat = -1;
}

void	init_philos(char **argv, t_philo *philos, t_waiter *waiter,
		pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	while (i < waiter->number_of_philosophers)
	{
		philo_parameters(i, argv, philos);
		if (philos[i].time_to_eat > philos[i].time_to_sleep)
			philos[i].time_to_think = (philos[i].time_to_eat
					- philos[i].time_to_sleep) + 1;
		else
			philos[i].time_to_think = 1;
		philos[i].id = i + 1;
		philos[i].is_eating = false;
		philos[i].start_time = get_current_time();
		philos[i].last_meal = get_current_time();
		philos[i].eaten_times = 0;
		philos[i].left_fork = &forks[i];
		philos[i].right_fork = &forks[(i + 1) % waiter->number_of_philosophers];
		philos[i].waiter = waiter;
		if (pthread_mutex_init(&philos[i].stomach, NULL) != 0)
			clean_error("Error : Failed to init mutex", waiter, forks, 5);
		i++;
	}
}

void	clean_forks(pthread_mutex_t *forks, int i)
{
	while (i > 0)
	{
		i--;
		pthread_mutex_destroy(&forks[i]);
	}
}

void	init_forks(pthread_mutex_t *forks, int philo_nbr, t_waiter *waiter)
{
	int	i;

	i = 0;
	while (i < philo_nbr)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
		{
			clean_forks(forks, i);
			clean_error("Error : Failed to init mutex", waiter, NULL, 4);
		}
		i++;
	}
}

void	init_waiter(t_waiter *waiter, int philo_nbr)
{
	waiter->number_of_philosophers = philo_nbr;
	waiter->time_to_stop = false;
	waiter->philos_full = 0;
	if (pthread_mutex_init(&waiter->full_mutex, NULL) != 0)
		clean_error("Error : Failed to init mutex", waiter, NULL, 0);
	if (pthread_mutex_init(&waiter->write_mutex, NULL) != 0)
		clean_error("Error : Failed to init mutex", waiter, NULL, 1);
	if (pthread_mutex_init(&waiter->dead_mutex, NULL) != 0)
		clean_error("Error : Failed to init mutex", waiter, NULL, 2);
	if (pthread_mutex_init(&waiter->eating_mutex, NULL) != 0)
		clean_error("Error : Failed to init mutex", waiter, NULL, 3);
}
