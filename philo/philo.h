#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/time.h>
#include <unistd.h>

#define PHILO_MAX 200

typedef struct s_philo
{
	pthread_t	philo_thread;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int number_of_times_each_philosopher_must_eat;
	int id;
    size_t			start_time;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t dead;
	pthread_mutex_t stomach;
    pthread_mutex_t	*write_mutex;
	int eaten_times;
}	t_philo;

typedef struct s_waiter
{
	pthread_t	waiter_thread;
	int	number_of_philosophers;
	bool dead_flag;
	pthread_mutex_t	dead_mutex;
    pthread_mutex_t	write_mutex;
}	t_waiter;

//PARSING//
typedef enum e_error
{
	NB_PHILO,
	VALUES_TOO_LOW,
	NB_ARGS,
	DIGITS_ARGS,
	LIMIT_INT_MAX
}	t_error;

void	ft_err(t_error error);

void init_philos(char **argv, t_philo *philos, t_waiter *waiter, pthread_mutex_t	*forks);
void init_forks(pthread_mutex_t	*forks, int philo_nbr);
void	init_waiter(t_waiter *waiter, int philo_nbr);

long ft_atol(const char *nptr);
int	ft_atoi(const char *nptr);

void    parsing(int argc, char **argv);
int	ft_isdigit(char c);
int	ft_isblank(int c);

size_t	get_current_time(void);
void	print_state_change(t_philo *philo, char *str);
