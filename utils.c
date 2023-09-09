#include "philo.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	realnb;
	int	minusorplus;

	i = 0;
	realnb = 0;
	minusorplus = 1;
	while (str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r' || str[i] == ' ')
	{
		i++;
	}
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			minusorplus *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		realnb = (realnb * 10) + (str[i] - 48);
		i++;
	}
	return (minusorplus * realnb);
}