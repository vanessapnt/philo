/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:53:23 by varodrig          #+#    #+#             */
/*   Updated: 2025/01/27 18:07:37 by varodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
philo >= 1

philo's behaviour:
- eat
- think
- sleep

forks = philo

eat(2 forks)

if 1 philo dies, exit

arguments :
- nb of philo
- time to die (eat-eat or start-eat) (ms)
- time to eat (between mutex ?)(ms)
- time to sleep (ms) usleep(time to sleep)
- optional 
*/

#include <pthread.h>
typedef struct s_philo
{
	int	number_of_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	number_of_times_each_philosopher_must_eat;
	pthread_mutex_t	left_fork;
	pthread_mutex_t	right_fork;
	pthread_mutex_t dead;
	pthread_mutex_t stomach;
	int	flag_fool;
	int	eaten_times;
}   t_philo;

typedef struct s_waiter
{
	t_philo *philos;
	int	flag_dead;
	//here so the philo who dies directly changes the varaibale of the waiter, the waiter doesn't have to check the flag_dead of everyone 
	pthread_mutex_t dead;
	pthread_mutex_t stomach;
	pthread_mutex_t write;
}	t_waiter;

int main()
{
	int	number_of_philosophers = 5;
	int	time_to_die = 2000;
	int	time_to_eat = 2000;
	int	time_to_sleep = 2000;
	int	number_of_times_each_philosopher_must_eat = 3;

	int N = number_of_philosophers;
	int i = 0;
	while(i < N)
	{
		
		i++;
	}

}
int forks[200] = {1};
while (1)
{
	//je lock R_fork si L_fork est dispo(1)
	if (fork[i+1] == 1)
	{
		mutex_lock(&fork[i]);
		fork[i] = 0;
		mutex_unlock(&fork[i])
	}
	else
		break;
	mutex_lock(&fork[i]);
	if fork[i] = 0;
	mutex_lock(&fork[i]);
}

//juste pour prevenir 
mutex_lock(&fork[i]);
waiter[i] = 0;
mutex_lock(&fork[i]);

eat()
{
	while(1)
	{
		if (!can_first_fork())
			break;
		
	}
}

//REDLIGHT//
//EVEN_PHILO_NB
time_to_wait = time_to_eat - time_to_sleep;
if (time_to_wait < 0)
    time_to_wait = 0;
if (i % 2 == 0)//even id
    philo_think(time_to_eat);
while (1)
{
    philo_eat();
    philo_sleep();
    philo_think(time_to_wait);  
}

//ODD_PHILO_NB
time_to_wait = time_to_eat * 2 - time_to_sleep;
if (i == 1)
    philo_think(time_to_eat * 2);
else if (i % 2 != 0)
    philo_think(time_to_eat);
while (1)
{
    philo_eat();
    philo_sleep();
    philo_think(time_to_wait);  
} 


