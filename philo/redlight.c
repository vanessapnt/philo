#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <bools.h>

typedef struct s_philo
{
	int	number_of_philosophers;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int number_of_times_each_philosopher_must_eat;
	int id;
	pthread_mutex_t	left_fork;
	pthread_mutex_t	right_fork;
	pthread_mutex_t dead;
	pthread_mutex_t stomach;
	int eaten_times;
}	t_philo;

typedef struct s_waiter
{
	bool dead_flag;
	pthread_mutex_t	dead_mutex;
}	t_waiter;

int	ft_isdigit(char c) ///TO CHANGE TO LIBFT
{
	return (c >= '0' && c <= '9');
}

static int	ft_isblank(int c)
{
	if ((c > 8 && c < 14) || (c == 32))
		return (1);
	return (0);
}

//PARSING//
typedef enum e_error
{
	NB_PHILO,
	VALUES_TOO_LOW,
	NB_ARGS,
	DIGITS_ARGS,
	LIMIT_INT_MAX
}	t_error;

void	ft_err(t_error error)
{
	if (error == NB_PHILO)
		printf("Too many philosophers. Try with max 200 philosophers\n");
	else if (error == VALUES_TOO_LOW)
		printf("The values must be integers and greater than 0.\n");
	else if (error == NB_ARGS)
		printf("Wrong number of arguments. The program must have 4 or 5 arguments.\n");
	else if (error == DIGITS_ARGS)
		printf("The arguments contain invalid characters.\n");
	else if (error == LIMIT_INT_MAX)
		printf("The values must be lower than INT_MAX.\n");
	exit(EXIT_FAILURE);
}



long ft_atol(const char *nptr)
{
    int i = 0;
    long num = 0;

    while (ft_isblank(nptr[i]) == 1 || nptr[i] == '+')
        i++;

    while (nptr[i] >= '0' && nptr[i] <= '9')
    {
        if (num > LONG_MAX / 10 || (num == LONG_MAX / 10 && (nptr[i] - '0') > 7))
            ft_err(LIMIT_INT_MAX);
        num = num * 10 + (nptr[i] - '0');
        i++;
    }
    return (num);
}


int	ft_atoi(const char *nptr) ///TO CHANGE TO LIBFT
{
	int	i;
	int	result;
	int	sign;

	i = 0;
	result = 0;
	sign = 1;
	while (ft_isblank(nptr[i]) == 1)
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (nptr[i] && ft_isdigit(nptr[i]) == 1)
	{
		result = (result * 10) + (nptr[i] - '0');
		i++;
	}
	return (result * sign);
}
int	ft_is_negative(char *str)
{
	int i;

	i = 0;
	while(str[i])
	{
		if (str[i] == '-')
			return (1);
		i++;
	}
	return (0);
}

int ft_only_digits(char *str)
{
    if (!str || *str == '\0')
		return (1);
    int i;
    i = 0;
    while (ft_isblank(str[i]) == 1)
		i++;
    if (str[i] == '+' || str[i] == '-')
        i++;
    while(str[i])
    {
        if (!ft_isdigit(str[i]))
            return (1);
        i++;
    }
    return (0);
}

void    parsing(int argc, char **argv)
{
    int i;
    if (argc != 5 && argc != 6)
        ft_err(NB_ARGS);
    i = 0;
    while (argv[++i])
    {
        if (ft_only_digits(argv[i]))
            ft_err(DIGITS_ARGS);
	}
	i = 0;
    while (argv[++i])
    {
        if (ft_is_negative(argv[i]))
            ft_err(VALUES_TOO_LOW);
        if (ft_atol(argv[i]) > INT_MAX)
            ft_err(LIMIT_INT_MAX);
    }
    if (ft_atoi(argv[1]) > 200)
        ft_err(NB_PHILO);
}

void init_philos(char **argv, t_philo *philos)
{
	int i;

	i = 1
	while (i <= argv[1])
	{
		philos[i].number_of_philosophers = ft_atoi(argv[1]);
		philos[i].time_to_die = ft_atoi(argv[2]);
		philos[i].time_to_eat = ft_atoi(argv[3]);
		philos[i].time_to_sleep = ft_atoi(argv[4]);
		if (argv[5])
			philos[i].num_times_to_eat = ft_atoi(argv[5]);
		else
			philos[i].num_times_to_eat = -1;
		philos[i].id = i;
		philos[i].eaten_times = 0;
		philos[i].left_fork = &forks[i];
		philos[i].right_fork = &forks[i % philo->number_of_philosophers + 1];
		philos[i].dead = &waiter->dead_flag;
		i++;
	}
}

void init_forks(pthread_mutex_t	*forks, int philo_nbr)
{
	int i;

	i = 1;
	while(i <= philo_nbr)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
}

void init_waiter()
{
	waiter.dead_flag = false;
}

int main(int argc, char **argv)
{
	t_philo			philos[PHILO_MAX];
	t_waiter		waiter;
	pthread_mutex_t	forks[PHILO_MAX];

	parsing(argc, argv);
	init_waiter();
	init_forks(forks, ft_atoi(argv[1]));
	init_philos(argv, &philos, &waiter);
	dinner(&philos);

	int	i;

	i = 0;
	while (int i = 0; i < 10; i++)
	{
		if (pthread_create(&th[i], NULL, &routine, NULL) != 0)
			perror("Failed to create thread");
	}
	while (int i = 0; i < 10; i++)
	{
	    if (pthread_join(th[i], NULL) != 0)
	        perror("Failed to join thread");
	}

	cleaning_dinner();
    return (0);
}
