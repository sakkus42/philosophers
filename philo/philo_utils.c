/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakkus <sakkus@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 08:58:26 by sakkus            #+#    #+#             */
/*   Updated: 2023/03/14 08:58:28 by sakkus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	fill_to_philo(t_philo *phil, pthread_mutex_t *forks,
							pthread_mutex_t *print, t_arg *arg)
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

	if (*phil->arg->is != 1)
	{
		pthread_mutex_lock(phil->mutex_philo);
		time = now_time_ms();
		printf("%lld ms %d %s\n", (time - phil->arg->time_ms), phil->phil_ind
			+ 0, str);
		if (death)
			*phil->arg->is = 1;
		pthread_mutex_unlock(phil->mutex_philo);
	}
}

int	is_dead(t_philo *phil)
{
	if (phil->arg->time_simu == phil->eaten)
		return (1);
	pthread_mutex_lock(phil->mutex_philo);
	if (*phil->arg->is == 1)
	{
		pthread_mutex_unlock(phil->mutex_philo);
		return (1);
	}
	pthread_mutex_unlock(phil->mutex_philo);
	if (phil->arg->nbr_of_philo == 1)
	{
		print_time("is dead", phil, 1);
		return (1);
	}
	if (now_time_ms() - phil->last_eat > phil->arg->time_die)
	{
		print_time("is dead", phil, 1);
		return (1);
	}
	return (0);
}
