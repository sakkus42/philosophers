#ifndef PHILO_H
# define PHILO_H

#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>

typedef struct s_arg
{
	int			nbr_of_philo; // philo sayısı
	int			time_die; // philo yemeye başlamamışsa ölme süresi
	int			time_eat; //philo yemek yeme süresi
	int			time_sleep; //philo uyuyarak geçireceği zaman
	int			time_simu; // optional argument simulasyon süresi
	long int	time_ms; // şu anki zamanın micro second hali
	int			is;
}	t_arg;

typedef struct	s_mutex
{
	pthread_mutex_t	mutex_Philo;
	pthread_mutex_t	mutex_fork_r;
	pthread_mutex_t	mutex_fork_l;
	pthread_mutex_t	mutex_first;
}	t_mutex;

typedef struct s_philo
{
	int				phil_ind;
	int				phil_time;
	t_arg			*arg;
	t_mutex			*mutex;
	pthread_t		th_philo;
}	t_philo;


# endif