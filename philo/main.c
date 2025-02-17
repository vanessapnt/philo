#include "philo.h"

/*
struct timeval {
    time_t      tv_sec;  // Seconds since January 1, 1970
    suseconds_t tv_usec; // Microseconds (0 to 999999)
};
*/

size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "error with gettimeofday()\n", 26);
	return (time.tv_sec * 1000 + time.tv_usec / 1000); //converts to ms
}

void	print_state_change(t_philo *philos, char *str)
{
	size_t	time;

	pthread_mutex_lock(philos->write_mutex);
	time = get_current_time() - philos->start_time;
	//if (!dead_loop(philo))
	printf("%zu %d %s\n", time, philos->id, str);
	pthread_mutex_unlock(philo->write_mutex);
}

static void	philo_thinks(t_philo *philos)
{
	print_state_change(philos, "is thinking");
}

static void	philo_sleeps(t_philo *philos)
{
	print_state_change(philos, "is sleeping");
}

static void	philo_eats(t_philo *philos)
{
	print_state_change(philos, "is eating");
}

void *waiter_routine(void *arg)
{
	t_waiter *waiter = (t_waiter *)arg;
	printf("Hello from waiter. Number of philosophers: %d\n", waiter->number_of_philosophers);
	return (NULL);
}

void *philo_routine(void *arg)
{
	t_philo *philos = (t_philo *)arg;
	printf("Hello from %d philo\n", philos->id);
	philo_eats(philos);
	philo_sleeps(philos);
	philo_thinks(philos);

	/*
	//REDLIGHT//
	//EVEN_PHILO_NB
	time_to_wait = time_to_eat - time_to_sleep;
	if (time_to_wait < 0)
		time_to_wait = 0;
	if (i % 2 == 0)//even id
		philo_philo_thinks(time_to_eat);
	while (1)
	{
		philo_philo_eats();
		philo_philo_sleeps();
		philo_philo_thinks(time_to_wait);  
	}

	//ODD_PHILO_NB
	time_to_wait = time_to_eat * 2 - time_to_sleep;
	if (i == 1)
		philo_philo_thinks(time_to_eat * 2);
	else if (i % 2 != 0)
		philo_philo_thinks(time_to_eat);
	while (1)
	{
		philo_philo_eats();
		philo_philo_sleeps();
		philo_philo_thinks(time_to_wait);  
	}
	*/
	return (NULL);
}

void	dinner(t_philo *philos, t_waiter *waiter)
{
	int	i;
	if (pthread_create(&waiter->waiter_thread, NULL, &waiter_routine, waiter) != 0)
	{
		perror("Failed to create thread");
		exit(1);
		//cleaning_dinner();
	}	
	i = 0;
	while (i < waiter->number_of_philosophers)
	{
		if (pthread_create(&philos[i].philo_thread, NULL, &philo_routine, &philos[i]) != 0)
		{
			perror("Failed to create thread");
			exit(1);
			//cleaning_dinner();
		}	
		i++;
	}
} 

int	main(int argc, char **argv)
{
	t_philo			philos[PHILO_MAX];
	t_waiter		waiter;
	pthread_mutex_t	forks[PHILO_MAX];
	int i;

	parsing(argc, argv);
	init_waiter(&waiter, ft_atoi(argv[1]));
	init_forks(forks, waiter.number_of_philosophers);
	init_philos(argv, philos, &waiter, forks);
	dinner(philos, &waiter);

	if (pthread_join(waiter.waiter_thread, NULL) != 0)
	{
		perror("Failed to create thread");
		//cleaning_dinner();
	}
	i = 0;
	while (i < waiter.number_of_philosophers)
	{
		if (pthread_join(philos[i].philo_thread, NULL) != 0)
		{
			perror("Failed to create thread");
			//cleaning_dinner();
		}	
		i++;
	}
	//cleaning_dinner();
	return (0);
}
