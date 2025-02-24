/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:57:42 by varodrig          #+#    #+#             */
/*   Updated: 2025/02/24 18:58:56 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean_mutexes(char *str, t_waiter *waiter, t_philo *philos,
		pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	if (str)
	{
		ft_putstr_fd(str, 2);
		ft_putstr_fd("\n", 2);
	}
	pthread_mutex_destroy(&waiter->dead_mutex);
	pthread_mutex_destroy(&waiter->write_mutex);
	pthread_mutex_destroy(&waiter->full_mutex);
	pthread_mutex_destroy(&philos->stomach);
	while (i < waiter->number_of_philosophers)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
}

void	dinner(t_philo *philos, t_waiter *waiter, pthread_mutex_t *forks)
{
	int	i;

	if (pthread_create(&waiter->waiter_thread, NULL, &waiter_routine,
			philos) != 0)
		clean_mutexes("Error : Failed to create threads", waiter, philos,
			forks);
	i = 0;
	while (i < waiter->number_of_philosophers)
	{
		if (pthread_create(&philos[i].philo_thread, NULL, &philo_routine,
				&philos[i]) != 0)
			clean_mutexes("Error : Failed to create threads", waiter, philos,
				forks);
		i++;
	}
	i = 0;
	if (pthread_join(waiter->waiter_thread, NULL) != 0)
		clean_mutexes("Error : Failed to join threads", waiter, philos, forks);
	while (i < waiter->number_of_philosophers)
	{
		if (pthread_join(philos[i].philo_thread, NULL) != 0)
			clean_mutexes("Error : Failed to join threads", waiter, philos,
				forks);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_philo			philos[PHILO_MAX];
	t_waiter		waiter;
	pthread_mutex_t	forks[PHILO_MAX];

	parsing(argc, argv);
	init_waiter(&waiter, ft_atoi(argv[1]));
	init_forks(forks, waiter.number_of_philosophers);
	init_philos(argv, philos, &waiter, forks);
	dinner(philos, &waiter, forks);
	clean_mutexes(NULL, &waiter, philos, forks);
	return (0);
}
