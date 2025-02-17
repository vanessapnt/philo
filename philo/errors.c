#include "philo.h"

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