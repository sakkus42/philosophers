#ifndef PHILO_H
# define PHILO_H

#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

typedef struct s_arg
{
	int	nbr_of_philo;
	int	time_die;
	int	time_eat;
	int	time_sleep;
	int	total_eat;
}	t_arg;

typedef struct s_philo
{
	int				phil_ind;
	pthread_t		philo;
	pthread_mutex_t	mutex_Philo;
}	t_philo;


# endif