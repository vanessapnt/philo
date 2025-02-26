/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 18:58:03 by varodrig          #+#    #+#             */
/*   Updated: 2025/02/26 20:56:57 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <errno.h>
# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define PHILO_MAX 200

typedef struct s_waiter
{
	pthread_t		waiter_thread;
	pthread_mutex_t	dead_mutex;
	pthread_mutex_t	write_mutex;
	pthread_mutex_t	full_mutex;
	pthread_mutex_t	eating_mutex;
	int				number_of_philosophers;
	bool			time_to_stop;
	int				philos_full;
}					t_waiter;

typedef struct s_philo
{
	pthread_t		philo_thread;
	int				id;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_think;
	bool			is_eating;
	int				max_eat;
	int				eaten_times;
	bool			dead;
	long long		start_time;
	long long		last_meal;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	stomach;
	t_waiter		*waiter;
}					t_philo;
// PARSING//
typedef enum e_error
{
	NB_PHILO,
	VALUES_TOO_LOW,
	NB_ARGS,
	DIGITS_ARGS,
	LIMIT_INT_MAX,
	ZERO_PHILO
}					t_error;

void				lock_forks(t_philo *philos);
void				unlock_forks(t_philo *philos);
int					philo_eats(t_philo *philos, int time_to_eat);

void				init_philos(char **argv, t_philo *philos, t_waiter *waiter,
						pthread_mutex_t *forks);
void				init_forks(pthread_mutex_t *forks, int philo_nbr,
						t_waiter *waiter);
void				init_waiter(t_waiter *waiter, int philo_nbr);

void				clean_mutexes(char *str, t_waiter *waiter, t_philo *philos,
						pthread_mutex_t *forks, int mode);
void				dinner(t_philo *philos, t_waiter *waiter,
						pthread_mutex_t *forks);
int					main(int argc, char **argv);

int					ft_isdigit(char c);
int					ft_isblank(int c);
void				parsing(int argc, char **argv);

int					philo_thinks(t_philo *philos, int time_to_think);
int					philo_sleeps(t_philo *philos, int time_to_sleep);
int					must_stop(t_philo *philos);
void				*philo_routine(void *arg);

void				ft_err(t_error error);
long long			get_current_time(void);
void				print_state_change(t_philo *philos, char *str);
int					manage_time(t_philo *philos, int time_to_wait);

long				ft_atol(const char *nptr);
int					ft_atoi(const char *nptr);
void				ft_putstr_fd(char *s, int fd);

int					philo_starved(t_philo *philos, long long time_to_die);
int					philo_dead(t_philo *philos);
int					everyone_full(t_philo *philos);
void				*waiter_routine(void *arg);

#endif