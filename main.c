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

int	checker_arg(int ac, char *arv[])
{
	int	i;

	i = 1;
	while (i < ac)
	{
		if (!is_dig(arv[i++]))
		{
			printf("only numeric values are valid\n");
			return (1);
		}
	}
	return (0);
}


int	chk_neg_atoi(char **arv)
{
	int	i;

	i = 1;
	while (arv[i])
	{
		if (ft_atoi(arv[i++]) <= 0)
			return (1);
	}
	return (0);
}

int	fill_to_arg(t_arg *arg, char **arv, int ac)
{
	if (chk_neg_atoi(arv))
		return (1);
	arg->nbr_of_philo = (int)ft_atoi(arv[1]);
	arg->time_die = (int)ft_atoi(arv[2]);
	arg->time_eat = (int)ft_atoi(arv[3]);
	arg->time_sleep = (int)ft_atoi(arv[4]);
	if (ac == 6)
		arg->total_eat = (int)ft_atoi(arv[5]);
	else
		arg->total_eat = 0;
	return (0);
}

int	fill_to_philo(t_philo *philo, t_arg *arg)
{
	int	i;

	i = 0;
	while (i < arg->nbr_of_philo)
	{
		philo[i].arg = malloc(sizeof(t_arg));
		philo[i].mutex_philo = 
	}
}

int main(int ac, char *arv[])
{
	t_arg	arg;
	t_philo	*philo;

	if (ac < 5 || ac > 6)
		return (0);
	if (checker_arg(ac, arv) || fill_to_arg(&arg, arv, ac))
		return (0);
	philo = malloc(sizeof(t_philo) * arg.nbr_of_philo);
	if (!philo)
		return (0);
	if (fill_to_philo(philo, &arg))
		return (0);

}