/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakkus <sakkus@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 09:06:09 by sakkus            #+#    #+#             */
/*   Updated: 2023/03/14 09:06:13 by sakkus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>

typedef long long int	t_time;

typedef struct s_arg
{
	int			nbr_of_philo;
	int			time_die;
	int			time_eat;
	int			time_sleep;
	int			time_simu;
	int			*is;
	t_time		time_ms;
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

void		wait_thread(int wait, t_philo *phil);
void		print_time(char *str, t_philo *phil, int death);
void		fill_to_philo(t_philo *phil, pthread_mutex_t *forks,
				pthread_mutex_t *print, t_arg *arg);
void		fill_to_arg(t_arg *arg, char **arv, int ac);
void		des_philo(t_philo *phil);
int			chk_neg_atoi(char **arv);
int			is_dead(t_philo *phil);
int			checker_arg(int ac, char *arv[]);
long int	ft_atoi(char *str);
t_time		now_time_ms(void);
void		des_philo(t_philo *phil);
#endif