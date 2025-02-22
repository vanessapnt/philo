#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/time.h>
#include <unistd.h>

#define PHILO_MAX 200

typedef struct s_waiter
{
	pthread_t       waiter_thread;
	pthread_mutex_t	dead_mutex;
    pthread_mutex_t	write_mutex;
	pthread_mutex_t	full_mutex;
    int             number_of_philosophers;
	bool            time_to_stop;
    int             philos_full;
}	t_waiter;

typedef struct s_philo
{
    pthread_t       philo_thread;
    int             id;
    int             time_to_die;
    int             time_to_eat;
    int             time_to_sleep;
    int             max_eat;
    int             eaten_times;
    bool            dead;
    size_t          start_time;
    size_t          last_meal;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    pthread_mutex_t stomach;
    t_waiter        *waiter;
}   t_philo;
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
int	must_stop(t_philo *philos);
