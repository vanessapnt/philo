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
	pthread_mutex_unlock(philos->write_mutex);
}

static void	philo_thinks(t_philo *philos, int time)
{
	print_state_change(philos, "is thinking");
	usleep(time * 1000);
}

static void	philo_sleeps(t_philo *philos, int time)
{
	print_state_change(philos, "is sleeping");
	//printf("%d\n", philos->time_to_sleep * 1000);
	usleep(time * 1000);
}

static void	philo_eats(t_philo *philos, int time)
{
	pthread_mutex_lock(philos->left_fork);
	print_state_change(philos, "has taken a fork");
	pthread_mutex_lock(philos->right_fork);
	print_state_change(philos, "has taken a fork");
	print_state_change(philos, "is eating");
	usleep(time * 1000);
	pthread_mutex_unlock(philos->left_fork);
	pthread_mutex_unlock(philos->right_fork);
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
	int	time_to_wait;
	int	i = philos->id;
	fprintf(stderr, "hello from %d\n", i);
	if (philos->waiter.number_of_philosophers % 2 == 0)
	{
		time_to_wait = philos->time_to_eat - philos->time_to_sleep;
		if (time_to_wait < 0)
			time_to_wait = 0;
		if (i % 2 == 0)//even id
			philo_thinks(philos, philos->time_to_eat);
		while (1)
		{
			philo_eats(philos, philos->time_to_eat);
			philo_sleeps(philos, philos->time_to_sleep);
			philo_thinks(philos, time_to_wait);  
		}
	}
	else
	{
		time_to_wait = philos->time_to_eat * 2 - philos->time_to_sleep;
		fprintf(stderr, "time to wait %d\n", time_to_wait);
		fprintf(stderr, "time to eat %d\n", philos->time_to_eat);
		fprintf(stderr, "time to sleep %d\n", philos->time_to_sleep);
		if (i == 1)
			philo_thinks(philos, philos->time_to_eat * 2);
		else if (i % 2 == 0)
			philo_thinks(philos, philos->time_to_eat);
		while (1)
		{
			philo_eats(philos, philos->time_to_eat);
			philo_sleeps(philos, philos->time_to_sleep);
			philo_thinks(philos, time_to_wait);  
		}
	}
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
	int j = -1;
	while(j <= 3)
	{
		////printf("fork %d = %p", j, (void*)&forks[j]);
		j++;
	}
	int i;

	parsing(argc, argv);
	init_waiter(&waiter, ft_atoi(argv[1]));
	init_forks(forks, waiter.number_of_philosophers);
	init_philos(argv, philos, &waiter, forks);
	fprintf(stderr, "%d %d %d\n", philos[0].id, philos[1].id, philos[2].id);
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
