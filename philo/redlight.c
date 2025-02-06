#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

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
		printf("Too many philosophers. Try with max 20 philosophers\n");
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

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

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

int main(int argc, char **argv)
{
    parsing(argc, argv);
    return (0);
}
