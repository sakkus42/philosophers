#include "philo.h"

long int	ft_atoi(char *str)
{
	int			i;
	long int	dig;

	i = 0;
	dig = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		dig = (dig * 10) + (str[i] - 48);
		i++;
	}
	if (str[0] == '-' || str[i] == '+')
		return (dig * -1);
	return (dig);
}

void    fill_to_arg(int ac, char **arv, t_arg *arg)
{
	arg->nbr_of_philo = (int)ft_atoi(arv[1]);
	arg->time_die = (int)ft_atoi(arv[2]);
	arg->time_eat = (int)ft_atoi(arv[3]);
	arg->time_sleep = (int)ft_atoi(arv[4]);
	if (ac == 6)
		arg->total_eat = (int)ft_atoi(arv[5]);
	else
		arg->total_eat = 0;
}

int		is_dig(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

void	checker_arg(int ac, char *arv[])
{
	int	i;

	i = 1;
	while (i < ac)
	{
		if (!is_dig(arv[i++]))
		{
			printf("only numeric values are valid\n");
			exit(1);
		}
	}
}

int main(int ac, char *arv[])
{
	t_arg	arg;

	if (ac < 5)
	{
		printf("missing argument\n");
		return (0);
	}
	checker_arg(ac, arv);
	fill_to_arg(ac, arv, &arg);
}