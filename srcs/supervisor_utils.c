/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervisor_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 21:55:36 by tkashi            #+#    #+#             */
/*   Updated: 2024/05/13 18:40:56 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	meals_supervisor(t_philo *philo_ptr, unsigned int *full_philos)
{
	t_data	*philo_data;

	philo_data = philo_ptr->data;
	if (philo_data->meals_present)
	{
		safe_mutex_handle(&philo_ptr->lock_meal_counter, LOCK);
		if (philo_ptr->meal_counter >= philo_data->meals_nbr)
		{
			safe_mutex_handle(&philo_ptr->lock_meal_counter, UNLOCK);
			(*full_philos)++;
		}
		else
			safe_mutex_handle(&philo_ptr->lock_meal_counter, UNLOCK);
	}
}

int	dead_supervisor(t_data *philo_data, unsigned int i)
{
	t_philo	*philo_ptr;

	philo_ptr = &(philo_data->philos[i]);
	safe_mutex_handle(&philo_ptr->lock_for_dead, LOCK);
	if (philo_ptr->dead == TRUE)
	{
		safe_mutex_handle(&philo_ptr->lock_for_dead, UNLOCK);
		kill_other_philos(philo_data, i);
		return (TRUE);
	}
	safe_mutex_handle(&philo_ptr->lock_for_dead, UNLOCK);
	return (FALSE);
}

void	ft_supervisor(t_data *philo_data)
{
	unsigned int	i;
	t_philo			*philo_ptr;
	unsigned int	full_philos;

	while (TRUE)
	{
		full_philos = 0;
		i = 0;
		while (i < philo_data->philos_nbr)
		{
			philo_ptr = &(philo_data->philos[i]);
			if (dead_supervisor(philo_data, i) == TRUE)
				return ;
			meals_supervisor(philo_ptr, &full_philos);
			i++;
		}
		if (full_philos == philo_data->philos_nbr)
		{
			kill_other_philos(philo_data, 0);
			return ;
		}
		ft_msleep(2);
	}
}
/*TODO: remove full message*/
