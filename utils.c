#include "philo.h"

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

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}
