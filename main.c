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

long int	now_time_ms()
{
	struct timeval	nw_time;
	long int		res;

	gettimeofday(&nw_time, NULL);
	res = (nw_time.tv_sec * 1000 + nw_time.tv_usec / 1000);	
	return (res);
}

void	fill_to_arg(t_arg *arg, char **arv, int ac)
{
	arg->nbr_of_philo = ft_atoi(arv[1]);
	arg->time_die = ft_atoi(arv[2]);
	arg->time_eat = ft_atoi(arv[3]);
	arg->time_sleep = ft_atoi(arv[4]);
	arg->is = 0;
	arg->time_ms = now_time_ms();
	if (ac == 6)
		arg->time_simu = ft_atoi(arv[5]);
	else
		arg->time_simu = 0;
}

void	fill_to_philo(t_philo *phil, pthread_mutex_t *forks, pthread_mutex_t *print, t_arg *arg)
{
	int	i;

	i = 0;
	while (i < arg->nbr_of_philo)
	{
		phil[i].phil_time = 0;
		phil[i].phil_ind = i + 1;
		phil[i].arg = arg;
		phil[i].mutex_left = &forks[i];
		phil[i].mutex_right = &forks[(i + 1) % arg->nbr_of_philo];
		printf("philo[%d] forks %d-%d\n", i, i,(i+1) % arg->nbr_of_philo);
		i++;
	}
	i = 0;
	while (i < arg->nbr_of_philo)
	{
		pthread_mutex_init(phil[i].mutex_left, NULL);
		pthread_mutex_init(phil[i].mutex_right, NULL);
		phil[i].mutex_philo = print;
		i++;
	}
	pthread_mutex_init(phil->mutex_philo, NULL);
}

// int	is_dead(t_philo *phil);

// int	print(char *s, t_philo *phil)
// {
// 	if (phil->arg->is == 1)
// 		return (1);
// 	if (is_dead(phil))
// 		return (1);	
// 	pthread_mutex_lock(phil->mutex_philo);
// 	printf("%d %s\n",phil->phil_ind, s);
// 	pthread_mutex_unlock(phil->mutex_philo);
// }

// int	is_dead(t_philo *phil)
// {
// 	if (phil->arg->is == 1)
// 		return (1);
// 	if (phil->phil_time + phil->arg->time_eat == phil->arg->time_die)
// 	{
// 		if (print("is dead", phil))
// 		{
// 			phil->arg->is = 1;
// 			return (1);
// 		}
// 	}
// 	else
// 		return (0);
// }


// int	eat(t_philo *phil)
// {
// 	if (phil->arg->is == 1)
// 		return (1);
// 	pthread_mutex_lock(phil->mutex_right);		
// 	if (print("has taken a fork", phil))
// 	{
// 		pthread_mutex_unlock(phil->mutex_right);
// 		return (1);
// 	}
// 	pthread_mutex_lock(phil->mutex_left);
// 	if (print("has taken a fork", phil))
// 	{
// 		pthread_mutex_unlock(phil->mutex_left);
// 		pthread_mutex_unlock(phil->mutex_right);	
// 		return (1);
// 	}
// 	if(print("is eating", phil))
// 	{
// 		pthread_mutex_unlock(phil->mutex_left);
// 		pthread_mutex_unlock(phil->mutex_right);	
// 		return (1);
// 	}
// 	phil->phil_time += phil->arg->time_eat;
// 	pthread_mutex_unlock(phil->mutex_left);
// 	pthread_mutex_unlock(phil->mutex_right);
// 	return (0);
// }

void	*house_of_philo(void *arg)
{
	t_philo	*phil;

	phil = (t_philo *)arg;
	pthread_mutex_lock(phil->mutex_right);
	printf("%d", phil->phil_ind);
	pthread_mutex_lock(phil->mutex_left);
	printf("%d\n", phil->phil_ind);
	pthread_mutex_unlock(phil->mutex_right);
	pthread_mutex_unlock(phil->mutex_left);
	usleep(100000);
}

void	init_thread(t_philo *phil)
{
	int	i;

	i = 0;
	while (i < phil->arg->nbr_of_philo)
	{
		pthread_create(&phil[i].th_philo, NULL, &house_of_philo, &phil[i]);
		i++;
		usleep(100);
	}
	i = 0;
	while (i < phil->arg->nbr_of_philo)
	{
		pthread_join(phil->th_philo, NULL);
		i++;
	}
}

int	main(int ac, char *arv[])
{
	t_arg			arg;
	t_philo			*philo;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*print;

	if (ac < 5 || ac > 6)
		return (0);
	if (chk_neg_atoi(arv) || checker_arg(ac, arv))
		return (0);
	philo = malloc(sizeof(t_philo) * ft_atoi(arv[1]));
	forks = malloc(sizeof(pthread_mutex_t) * ft_atoi(arv[1]));
	print = malloc(sizeof(pthread_mutex_t));
	if (!philo || !forks || !print)
		return (0);
	fill_to_arg(&arg, arv, ac);
	fill_to_philo(philo, forks, print, &arg);
	init_thread(philo);
}