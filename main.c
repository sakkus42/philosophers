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
	res = ((nw_time.tv_sec * 1000) * 1000) + (nw_time.tv_usec * 1000);	
	return (res);
}

int	fill_to_arg(t_arg *arg, char **arv, int ac)
{
	if (chk_neg_atoi(arv))
		return (1);
	arg->nbr_of_philo = (int)ft_atoi(arv[1]);
	arg->time_die = (int)ft_atoi(arv[2]);
	arg->time_eat = (int)ft_atoi(arv[3]);
	arg->time_sleep = (int)ft_atoi(arv[4]);
	arg->time_ms = now_time_ms();
	arg->phil_time = 0;
	if (ac == 6)
		arg->time_simu = (int)ft_atoi(arv[5]);
	else
		arg->time_simu = 0;
	return (0);
}


int	fill_to_philo(t_philo *philo, t_arg *arg, t_mutex *mutex)
{
	int	i;

	i = 0;
	while (i < arg->nbr_of_philo)
	{
		philo[i].arg = arg;
		philo[i].mutex = mutex;
		philo[i].phil_ind = i + 1;
		i++;
	}
	return (0);
}

void	des_mutex(t_mutex *mutex)
{
	pthread_mutex_destroy(&mutex->mutex_fork_l);
	pthread_mutex_destroy(&mutex->mutex_fork_r);
	pthread_mutex_destroy(&mutex->mutex_Philo);
}

void	exit_datafree(t_philo *philo)
{
	if (philo)
	{
		des_mutex(philo->mutex);
	}
	exit(1);
}


void	init_mutex(t_mutex *mutex)
{
	pthread_mutex_init(&mutex->mutex_fork_l, NULL);
	pthread_mutex_init(&mutex->mutex_fork_r, NULL);
	pthread_mutex_init(&mutex->mutex_Philo, NULL);
}

void	is_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutex->mutex_Philo);
	if (philo->arg->phil_time + philo->arg->time_eat == philo->arg->time_die)
	{
		printf("else if philo[%d] => %d died \n", philo->phil_ind, philo->arg->phil_time);
		exit_datafree(philo);
	}
	pthread_mutex_unlock(&philo->mutex->mutex_Philo);
}

void	eat(t_philo *philo)
{
	// printf("philo_id_fun:	%d\n", philo->phil_ind);
	usleep(200000);
	if (philo->phil_ind % 2 && philo->arg->nbr_of_philo != philo->phil_ind)
	{
		pthread_mutex_lock(&philo->mutex->mutex_fork_l);
		is_dead(philo);
		printf("philo[%d]:	%d ms has taken a fork\n", philo->phil_ind, philo->arg->phil_time);
		pthread_mutex_lock(&philo->mutex->mutex_fork_r);
		printf("philo[%d]:	%d ms has taken a fork\n", philo->phil_ind, philo->arg->phil_time);
		printf("philo[%d]:	%d ms has is eating\n", philo->phil_ind, philo->arg->time_eat);
		philo->arg->phil_time += philo->arg->time_eat;
		pthread_mutex_unlock(&philo->mutex->mutex_fork_l);
		pthread_mutex_unlock(&philo->mutex->mutex_fork_r);
	}
	else
	{
		pthread_mutex_lock(&philo->mutex->mutex_fork_l);
		printf("philo[%d]:	%d ms has taken a fork\n", philo->phil_ind, philo->arg->phil_time);
		pthread_mutex_lock(&philo->mutex->mutex_fork_r);
		is_dead(philo);
		printf("philo[%d]:	%d ms has taken a fork\n", philo->phil_ind, philo->arg->phil_time);
		printf("philo[%d]:	%d ms has is eating\n", philo->phil_ind, philo->arg->time_eat);
		philo->arg->phil_time += philo->arg->time_eat;
		pthread_mutex_unlock(&philo->mutex->mutex_fork_l);
		pthread_mutex_unlock(&philo->mutex->mutex_fork_r);
	}
	usleep(200000);
}

void	*house_of_philo(void *arg)
{
	int i = 0; 
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->arg->phil_time == philo->arg->time_die || philo->arg->time_die <= philo->arg->time_eat)
		return (0);
	while (1)
	{
		is_dead(philo);
		eat(philo);
	}
	printf("philo_time:	%d", philo->arg->phil_time);
	return (0);
}

void	init_thread(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->arg->nbr_of_philo)
	{
		// printf("philo_id :	%d\n", philo[i].phil_ind);
		if (pthread_create(&philo[i].th_philo, NULL, house_of_philo, &philo[i]) != 0)
			exit_datafree(philo);
		i++;
	}
	i =0;
	while (i < philo->arg->nbr_of_philo)
	{
		if (pthread_join(philo[i].th_philo, NULL) != 0)
			exit_datafree(philo);
		i++;
	}
	
}

int main(int ac, char *arv[])
{
	t_arg	arg;
	t_mutex	mutex;
	t_philo	*philo;

	if (ac < 5 || ac > 6)
		return (0);
	if (checker_arg(ac, arv) || fill_to_arg(&arg, arv, ac))
		return (0);
	philo = malloc(sizeof(t_philo) * arg.nbr_of_philo);
	if (!philo)
		return (0);
	init_mutex(&mutex);
	if (fill_to_philo(philo, &arg, &mutex))
		return (0);
	init_thread(philo);
}