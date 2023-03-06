#ifndef PHILO_H
# define PHILO_H

#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>

typedef struct s_arg
{
	int	nbr_of_philo; // philo sayısı
	int	time_die; // philo yemeye başlamamışsa ölme süresi
	int	time_eat; //philo yemek yeme süresi
	int	time_sleep; //philo uyuyarak geçireceği zaman
	int	total_eat; // optional argument simulasyon süresi
}	t_arg;

typedef struct s_philo
{
	int				m_second;
	int				phil_ind;
	pthread_t		th_philo;
	pthread_mutex_t	mutex_Philo;
	pthread_mutex_t	mutex_fork_r;
	pthread_mutex_t	mutex_fork_l;
	t_arg			*arg;
}	t_philo;


# endif