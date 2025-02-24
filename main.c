#include "philo.h"

/*
struct timeval {
	time_t      tv_sec;  // Seconds since January 1, 1970
	suseconds_t tv_usec; // Microseconds (0 to 999999)
};
*/

long long get_current_time(void)
{
	struct timeval tv;
	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return ((tv.tv_sec) * (long long)1000 + (tv.tv_usec) / 1000);
}

void print_state_change(t_philo *philos, char *str)
{
	long long time;
	
	pthread_mutex_lock(&philos->waiter->write_mutex);
	if (!must_stop(philos))
	{
		time = get_current_time() - philos->start_time;
		printf("%lld %d %s\n", time, philos->id, str);
	}
	pthread_mutex_unlock(&philos->waiter->write_mutex);
}

int manage_time(t_philo *philos, int time_to_wait) {
	long long start_time;
	long long elapsed_time;

	start_time = get_current_time();
	while (1) {
		usleep(50);
		elapsed_time = get_current_time() - start_time;
		if (elapsed_time >= time_to_wait)
			break;
		if (must_stop(philos)) {
			return (1);
		}
	}
	usleep(500);
	return (0);
}

void	clean_mutexes(char *str, t_waiter *waiter, t_philo *philos, pthread_mutex_t *forks)
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

int philo_thinks(t_philo *philos, int time_to_think)
{
	if (must_stop(philos))
		return (1);
	print_state_change(philos, "is thinking​​💭​");
	if (manage_time(philos, time_to_think) != 0) {
		return (1);
	}
	return (0);
}

int philo_sleeps(t_philo *philos, int time_to_sleep)
{
	if (must_stop(philos))
		return (1);
	print_state_change(philos, "is sleeping​💤​");
	if (manage_time(philos, time_to_sleep) != 0) {
		return (1);
	}
	return (0);
}

void lock_forks(t_philo *philos) {
	if (philos->id % 2 == 1) {
		usleep(100);
		pthread_mutex_lock(philos->left_fork);
		print_state_change(philos, "has taken a fork");
		pthread_mutex_lock(philos->right_fork);
		print_state_change(philos, "has taken a fork");
	} else {
		pthread_mutex_lock(philos->right_fork);
		print_state_change(philos, "has taken a fork");
		pthread_mutex_lock(philos->left_fork);
		print_state_change(philos, "has taken a fork");
	}
}

void unlock_forks(t_philo *philos) {
	usleep(100);
	pthread_mutex_unlock(philos->left_fork);
	pthread_mutex_unlock(philos->right_fork);
}

/*
int philo_eats(t_philo *philos, int time_to_eat)
{
	if (must_stop(philos))
		return (1);
	if (philos->waiter->number_of_philosophers == 1)
	{
		pthread_mutex_lock(philos->left_fork);
		print_state_change(philos, "has taken a fork");
		pthread_mutex_unlock(philos->left_fork);
		return (1);
	}
	lock_forks(philos);
	pthread_mutex_lock(&philos->stomach);
	philos->last_meal = get_current_time();
	pthread_mutex_unlock(&philos->stomach);
	if (must_stop(philos))
	{
		unlock_forks(philos);
		return (1);
	}
	print_state_change(philos, "is eating🍖");
	pthread_mutex_lock(&philos->stomach);
	philos->eaten_times++;
	if (philos->max_eat != -1 && philos->eaten_times >= philos->max_eat)
	{
		pthread_mutex_lock(&philos->waiter->full_mutex);
		philos->waiter->philos_full++;
		pthread_mutex_unlock(&philos->waiter->full_mutex);
	}
	pthread_mutex_unlock(&philos->stomach);
	usleep(time_to_eat * 1000);
	unlock_forks(philos);
	return (0);
}
*/

int philo_eats(t_philo *philos, int time_to_eat) {
	if (must_stop(philos))
		return (1);
	if (philos->waiter->number_of_philosophers == 1)
	{
		pthread_mutex_lock(philos->left_fork);
		print_state_change(philos, "has taken a fork");
		pthread_mutex_unlock(philos->left_fork);
		return (1);
	}
	lock_forks(philos);
	pthread_mutex_lock(&philos->stomach);
	philos->last_meal = get_current_time();
	pthread_mutex_unlock(&philos->stomach);
	if (must_stop(philos)) {
		unlock_forks(philos);
		return (1);
	}
	print_state_change(philos, "is eating🍖");
	pthread_mutex_lock(&philos->stomach);
	philos->eaten_times++;
	if (philos->max_eat != -1 && philos->eaten_times >= philos->max_eat) {
		pthread_mutex_lock(&philos->waiter->full_mutex);
		philos->waiter->philos_full++;
		pthread_mutex_unlock(&philos->waiter->full_mutex);
	}
	pthread_mutex_unlock(&philos->stomach);

	if (manage_time(philos, time_to_eat) != 0) {
		unlock_forks(philos);
		return (1);
	}
	unlock_forks(philos);
	return (0);
}


int philo_starved(t_philo *philos, long long time_to_die)
{
	long long time_since_last_meal;
	
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
		usleep(1000);
	}
	return (NULL);
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
			break; 
		if (philo_sleeps(philos, philos->time_to_sleep))
			break;
		if (philo_thinks(philos, philos->time_to_think))
			break;
	}
	return (NULL);
}
void	dinner(t_philo *philos, t_waiter *waiter, pthread_mutex_t *forks)
{
	int	i;
	if (pthread_create(&waiter->waiter_thread, NULL, &waiter_routine, philos) != 0)
		clean_mutexes("Error : Failed to create threads", waiter, philos, forks);	
	i = 0;
	while (i < waiter->number_of_philosophers)
	{
		if (pthread_create(&philos[i].philo_thread, NULL, &philo_routine, &philos[i]) != 0)
			clean_mutexes("Error : Failed to create threads", waiter, philos, forks);	
		i++;
	}
	i = 0;
	if (pthread_join(waiter->waiter_thread, NULL) != 0)
		clean_mutexes("Error : Failed to join threads", waiter, philos, forks);	
	while (i < waiter->number_of_philosophers)
	{
		if (pthread_join(philos[i].philo_thread, NULL) != 0)
			clean_mutexes("Error : Failed to join threads", waiter, philos, forks);	
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
