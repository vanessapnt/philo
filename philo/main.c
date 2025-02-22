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

void print_state_change(t_philo *philos, char *str)
{
    size_t time;
    
    pthread_mutex_lock(&philos->waiter->write_mutex);
    if (!must_stop(philos))
    {
        time = get_current_time() - philos->start_time;
        printf("%zu %d %s\n", time, philos->id, str);
    }
    pthread_mutex_unlock(&philos->waiter->write_mutex);
}
/*
void	philo_thinks(t_philo *philos)
{
	print_state_change(philos, "is thinking");
}

void	philo_sleeps(t_philo *philos)
{
	print_state_change(philos, "is sleeping");
	//printf("%d\n", philos->time_to_sleep * 1000);
	usleep(philos->time_to_sleep * 1000);
}

void	philo_eats(t_philo *philos)
{
	pthread_mutex_lock(philos->left_fork);
	print_state_change(philos, "has taken a fork");
	pthread_mutex_lock(philos->right_fork);
	print_state_change(philos, "has taken a fork");
	print_state_change(philos, "is eating");
	usleep(philos->time_to_eat * 1000);
	pthread_mutex_lock(&philos->stomach);
	philos->last_meal = get_current_time();
	philos->eaten_times++;
	if (philos->eaten_times == philos->max_eat)
	{
		pthread_mutex_lock(&philos->waiter->full_mutex);
		philos->waiter->philos_full++;
		pthread_mutex_unlock(&philos->waiter->full_mutex);
	}
	pthread_mutex_unlock(&philos->stomach);
	pthread_mutex_unlock(philos->left_fork);
	pthread_mutex_unlock(philos->right_fork);
}
*/

int philo_thinks(t_philo *philos, int time_to_wait)
{
    if (must_stop(philos))
        return (0);
    print_state_change(philos, "is thinking​​💭​");
    if (time_to_wait > 0)
        usleep(time_to_wait * 1000);
    return (1);
}

int philo_sleeps(t_philo *philos, int time_to_sleep)
{
    if (must_stop(philos))
        return (0);
    print_state_change(philos, "is sleeping​💤​");
    usleep(time_to_sleep * 1000);
    return (1);
}

int philo_eats(t_philo *philos, int time_to_eat)
{
    if (must_stop(philos))
        return (0);
    pthread_mutex_lock(philos->left_fork);
    print_state_change(philos, "has taken a fork");
    pthread_mutex_lock(philos->right_fork);
    print_state_change(philos, "has taken a fork");
    print_state_change(philos, "is eating🍖​");
    
    pthread_mutex_lock(&philos->stomach);
    philos->last_meal = get_current_time();
    philos->eaten_times++;
    pthread_mutex_unlock(&philos->stomach);
    
    usleep(time_to_eat * 1000);
    pthread_mutex_unlock(philos->left_fork);
    pthread_mutex_unlock(philos->right_fork);
    return (1);
}

int philo_starved(t_philo *philos, size_t time_to_die)
{
    size_t time_since_last_meal;
    
    pthread_mutex_lock(&philos->stomach);
    time_since_last_meal = get_current_time() - philos->last_meal;
    pthread_mutex_unlock(&philos->stomach);
    
    return (time_since_last_meal >= time_to_die);
}

int	philo_dead(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos->waiter->number_of_philosophers)
	{
		if (philo_starved(&philos[i], philos[i].time_to_die))
		{
			print_state_change(&philos[i], "died​​​☠️​");
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
		return(1);
	}
	pthread_mutex_unlock(&philos->waiter->full_mutex);
	return(0);
}

void *waiter_routine(void *arg)
{
	t_philo *philos = (t_philo *)arg;
	while(1)
	{
		if (philo_dead(philos) || everyone_full(philos))
		{
			pthread_mutex_lock(&philos->waiter->dead_mutex);
			philos->waiter->time_to_stop = true;
			pthread_mutex_unlock(&philos->waiter->dead_mutex);
			break;
		}
		usleep(100);
	}
	return (arg);
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
/*
void	*philo_routine(void *arg)
{
	t_philo	*philos;

	philos = (t_philo *)arg;
	if (philos->id % 2 == 0)
		usleep(1000);
	while (!must_stop(philos))
	{
		philo_eats(philos);
		philo_sleeps(philos);
		philo_thinks(philos);
	}
	return (arg);
}
*/

void *philo_routine(void *arg)
{
    t_philo *philos = (t_philo *)arg;
    int time_to_wait;
    int i = philos->id;
    
    if (philos->waiter->number_of_philosophers % 2 == 0)
    {
        time_to_wait = philos->time_to_eat - philos->time_to_sleep;
        if (time_to_wait < 0)
            time_to_wait = 0;
        
        if (i % 2 == 0) //even id
            usleep(philos->time_to_eat * 1000); // Juste attendre au lieu de "think"
            
        while (!must_stop(philos))
        {
            if (!philo_eats(philos, philos->time_to_eat))
                break;
            if (!philo_sleeps(philos, philos->time_to_sleep))
                break;
            if (!philo_thinks(philos, 0)) // Ne pas attendre pendant thinking
                break;
        }
    }
    else
    {
        time_to_wait = philos->time_to_eat * 2 - philos->time_to_sleep;        
        if (i == 1)
            usleep((philos->time_to_eat * 2) * 1000); // Juste attendre
        else if (i % 2 == 0)
            usleep(philos->time_to_eat * 1000); // Juste attendre
        
        while (!must_stop(philos))
        {
            if (!philo_eats(philos, philos->time_to_eat))
                break;
            if (!philo_sleeps(philos, philos->time_to_sleep))
                break;
            if (!philo_thinks(philos, 0)) // Ne pas attendre pendant thinking
                break;
        }
    }
    return (arg);
}

/*
void *philo_routine(void *arg)
{
	t_philo *philos = (t_philo *)arg;
	int	time_to_wait;
	int	i = philos->id;
	fprintf(stderr, "hello from %d\n", i);
	if (philos->waiter->number_of_philosophers % 2 == 0)
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
	return (arg);
}
*/

void	dinner(t_philo *philos, t_waiter *waiter)
{
	int	i;
	if (pthread_create(&waiter->waiter_thread, NULL, &waiter_routine, philos) != 0)
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
