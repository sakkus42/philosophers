/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakkus <sakkus@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 09:09:12 by sakkus            #+#    #+#             */
/*   Updated: 2023/03/14 09:09:16 by sakkus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	des_philo(t_philo *phil)
{
	int	i;

	i = 0;
	while (i < phil->arg->nbr_of_philo)
		pthread_mutex_destroy(phil[i++].mutex_left);
	free(phil->arg->is);
	free(phil->mutex_philo);
	free(phil);
}
