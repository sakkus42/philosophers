#ifndef PHILO_H
# define PHILO_H

#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>

typedef long long int t_time;

typedef struct s_arg
{
	int			nbr_of_philo; // philo sayısı
	int			time_die; // philo yemeye başlamamışsa ölme süresi
	int			time_eat; //philo yemek yeme süresi
	int			time_sleep; //philo uyuyarak geçireceği zaman
	int			time_simu; // optional argument simulasyon süresi
	int			*is;
	t_time		time_ms; // şu anki zamanın mili second hali
}	t_arg;

typedef struct s_philo
{
	t_arg			*arg;
	int				phil_ind;
	int				eaten;
	t_time			last_eat;
	pthread_mutex_t	*mutex_philo;
	pthread_mutex_t	*mutex_right;
	pthread_mutex_t	*mutex_left;
	pthread_t		th_philo;
}	t_philo;


# endif