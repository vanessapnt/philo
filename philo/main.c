/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:57:42 by varodrig          #+#    #+#             */
/*   Updated: 2025/02/27 12:51:40 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean_error(char *str, t_waiter *waiter, pthread_mutex_t *forks,
		int mode)
{
	int	i;

	if (str)
		write_error(str);
	if (mode >= 1)
		pthread_mutex_destroy(&waiter->full_mutex);
	if (mode >= 2)
		pthread_mutex_destroy(&waiter->write_mutex);
	if (mode >= 3)
		pthread_mutex_destroy(&waiter->dead_mutex);
	if (mode >= 4)
		pthread_mutex_destroy(&waiter->eating_mutex);
	i = 0;
	if (mode >= 5)
	{
		while (i < waiter->number_of_philosophers)
		{
			pthread_mutex_destroy(&forks[i]);
			i++;
		}
	}
	if (mode >= 6)
		return ;
	exit(1);
}

static void	ft_join(t_waiter *waiter, t_philo *philos, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	if (pthread_join(waiter->waiter_thread, NULL) != 0)
	{
		clean_error("Error : Failed to join threads", waiter, forks, 6);
		clean_remaining(philos, waiter);
	}
	while (i < waiter->number_of_philosophers)
	{
		if (pthread_join(philos[i].philo_thread, NULL) != 0)
		{
			clean_error("Error : Failed to join threads", waiter, forks, 6);
			clean_remaining(philos, waiter);
		}
		i++;
	}
}

void	dinner(t_philo *philos, t_waiter *waiter, pthread_mutex_t *forks)
{
	int	i;

	if (pthread_create(&waiter->waiter_thread, NULL, &waiter_routine,
			philos) != 0)
	{
		clean_error("Error : Failed to create threads", waiter, forks, 6);
		clean_remaining(philos, waiter);
	}
	i = 0;
	while (i < waiter->number_of_philosophers)
	{
		if (pthread_create(&philos[i].philo_thread, NULL, &philo_routine,
				&philos[i]) != 0)
		{
			clean_error("Error : Failed to create threads", waiter, forks, 6);
			clean_remaining(philos, waiter);
		}
		i++;
	}
	ft_join(waiter, philos, forks);
}

int	main(int argc, char **argv)
{
	t_philo			philos[PHILO_MAX];
	t_waiter		waiter;
	pthread_mutex_t	forks[PHILO_MAX];

	parsing(argc, argv);
	init_waiter(&waiter, ft_atoi(argv[1]));
	init_forks(forks, waiter.number_of_philosophers, &waiter);
	init_philos(argv, philos, &waiter, forks);
	dinner(philos, &waiter, forks);
	clean_error(NULL, &waiter, forks, 6);
	clean_remaining(philos, &waiter);
	return (0);
}
