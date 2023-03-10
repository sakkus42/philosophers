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

t_time	now_time_ms()
{
	struct timeval	nw_time;
	t_time			res;

	gettimeofday(&nw_time, NULL);
	res = (nw_time.tv_sec * 1000 + nw_time.tv_usec / 1000);	
	return (res);
}

void	fill_to_arg(t_arg *arg, char **arv, int ac)
{
	int	*is;

	is = malloc(sizeof(int));
	*is = 0;
	arg->nbr_of_philo = ft_atoi(arv[1]);
	arg->time_die = ft_atoi(arv[2]);
	arg->time_eat = ft_atoi(arv[3]);
	arg->time_sleep = ft_atoi(arv[4]);
	arg->is = is;
	arg->time_ms = now_time_ms();
	if (ac == 6)
		arg->time_simu = ft_atoi(arv[5]);
	else
		arg->time_simu = -1;
}

void	fill_to_philo(t_philo *phil, pthread_mutex_t *forks, pthread_mutex_t *print, t_arg *arg)
{
	int	i;

	i = 0;
	while (i < arg->nbr_of_philo)
	{
		phil[i].last_eat = now_time_ms();
		phil[i].phil_ind = i + 1;
		phil[i].eaten = 0;
		phil[i].arg = arg;
		phil[i].mutex_left = &forks[i];
		phil[i].mutex_right = &forks[(i + 1) % arg->nbr_of_philo];
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

int	is_dead(t_philo *phil);

void	wait_thread(int wait, t_philo *phil)
{
	t_time	time;

	time = now_time_ms();
	while (now_time_ms() - time < (t_time)wait)
	{
		if (is_dead(phil))
			return ;
		usleep(100);
	}
}

void	print_time(char *str, t_philo *phil, int death)
{
	t_time	time;

	if (*phil->arg->is != 1 || death == 1)
	{
		time = now_time_ms();
		pthread_mutex_lock(phil->mutex_philo);
		if (death == 1)
			death = 0;
		printf("%lld ms %d %s\n", time - phil->arg->time_ms, phil->phil_ind, str);
		pthread_mutex_unlock(phil->mutex_philo);
	}
}

int	is_dead(t_philo *phil)
{
	if (phil->arg->nbr_of_philo == 1)
	{
		*phil->arg->is = 1;
		print_time("is dead", phil, 1);
		return (1);
	}
	if (*phil->arg->is == 1)
		return (1);
	pthread_mutex_lock(phil->mutex_philo);
	// printf("%lld sonuc %d\n", now_time_ms() - phil->last_eat, phil->arg->time_die);
	if (phil->arg->time_simu == phil->eaten)
	{
		pthread_mutex_unlock(phil->mutex_philo);
		return (1);
	}
	if (now_time_ms() - phil->last_eat > phil->arg->time_die)
	{
		*phil->arg->is = 1;
		pthread_mutex_unlock(phil->mutex_philo);
		print_time("is dead", phil, 1);
		return (1);
	}
	pthread_mutex_unlock(phil->mutex_philo);
	return (0);
}

int	eat(t_philo *phil)
{
	if (*phil->arg->is == 1)
		return (1);
	pthread_mutex_lock(phil->mutex_left);
	print_time("has left a fork", phil, 0);
	if (is_dead(phil))
	{
		pthread_mutex_unlock(phil->mutex_left);
		return (1);
	}
	pthread_mutex_lock(phil->mutex_right);
	print_time("has right a fork", phil, 0);
	if (is_dead(phil))
	{
		pthread_mutex_unlock(phil->mutex_left);
		pthread_mutex_unlock(phil->mutex_right);
		return (1);
	}
	print_time("is eating", phil, 0);
	wait_thread(phil->arg->time_eat, phil);
	phil->eaten++;
	phil->last_eat = now_time_ms();
	return (0);
}

int	sleeping(t_philo *phil)
{
	pthread_mutex_unlock(phil->mutex_left);
	pthread_mutex_unlock(phil->mutex_right);
	if (is_dead(phil))
		return (1);
	wait_thread(phil->arg->time_sleep, phil);
	print_time("is sleeping", phil, 0);
	return (0);
}

void	*house_of_philo(void *arg)
{
	t_philo *phil;

	phil = (t_philo *)arg;
	while (1)
	{
		if(eat(phil))
			break ;
		if (sleeping(phil))
			break ;
		if (is_dead(phil) == 0)
			print_time("is thinking", phil, 0);
	}
	return (0);
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
		pthread_join(phil[i].th_philo, NULL);
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