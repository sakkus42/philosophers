#include "philo.h"

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