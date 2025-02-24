#include "philo.h"

void init_philos(char **argv, t_philo *philos, t_waiter *waiter, pthread_mutex_t *forks)
{
    int i;

    i = 0;
    while (i < waiter->number_of_philosophers)
    {
        philos[i].time_to_die = ft_atoi(argv[2]);
        philos[i].time_to_eat = ft_atoi(argv[3]);
        philos[i].time_to_sleep = ft_atoi(argv[4]);
        if (argv[5])
            philos[i].max_eat = ft_atoi(argv[5]);
        else
            philos[i].max_eat = -1;
        if (philos[i].time_to_eat > philos[i].time_to_sleep)
		    philos[i].time_to_think = (philos[i].time_to_eat - philos[i].time_to_sleep) + 1;
        else
		    philos[i].time_to_think = 1;
        philos[i].id = i + 1;
        philos[i].start_time = get_current_time();
        philos[i].last_meal = philos[i].start_time;
        philos[i].eaten_times = 0;
        philos[i].left_fork = &forks[i];
        philos[i].right_fork = &forks[(i + 1) % waiter->number_of_philosophers];
        philos[i].waiter = waiter;
        pthread_mutex_init(&philos[i].stomach, NULL);
        i++;
    }
}

void init_forks(pthread_mutex_t	*forks, int philo_nbr)
{
	int i;

	i = 0;
	while(i < philo_nbr)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
}

void init_waiter(t_waiter *waiter, int philo_nbr)
{
    waiter->number_of_philosophers = philo_nbr;
    waiter->time_to_stop = false;
    waiter->philos_full = 0;
    pthread_mutex_init(&waiter->full_mutex, NULL);
    pthread_mutex_init(&waiter->write_mutex, NULL);
    pthread_mutex_init(&waiter->dead_mutex, NULL);
}
