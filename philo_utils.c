#include "philo.h"

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