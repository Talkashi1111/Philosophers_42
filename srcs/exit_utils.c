/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 20:48:37 by tkashi            #+#    #+#             */
/*   Updated: 2024/05/13 18:39:16 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	free_data(t_data *philo_data)
{
	if (philo_data->forks)
	{
		free(philo_data->forks);
		philo_data->forks = NULL;
	}
	if (philo_data->philos)
	{
		free(philo_data->philos);
		philo_data->philos = NULL;
	}
}

void	destroy_fork_mutexes(t_data *philo_data)
{
	unsigned int	i;

	i = 0;
	while (i < philo_data->philos_nbr)
	{
		safe_mutex_handle(&philo_data->forks[i], DESTROY);
		i++;
	}
}

void	destroy_mutexes(t_data *philo_data)
{
	unsigned int	i;

	i = 0;
	while (i < philo_data->philos_nbr)
	{
		safe_mutex_handle(&philo_data->philos[i].lock_meal_counter, DESTROY);
		safe_mutex_handle(&philo_data->philos[i].lock_for_dead, DESTROY);
		i++;
	}
	destroy_fork_mutexes(philo_data);
	safe_mutex_handle(&philo_data->lock_print, DESTROY);
}

void	revert_threads(t_data *philo_data, unsigned int idx)
{
	unsigned int	i;
	t_philo			*philo_ptr;

	i = idx;
	while (i > 0)
	{
		--i;
		philo_ptr = &(philo_data->philos[i]);
		safe_mutex_handle(&philo_ptr->lock_for_dead, LOCK);
		philo_ptr->dead = TRUE;
		safe_mutex_handle(&philo_ptr->lock_for_dead, UNLOCK);
	}
	i = idx;
	while (i > 0)
	{
		--i;
		philo_ptr = &(philo_data->philos[i]);
		safe_thread_handle(&philo_ptr->thread_id, NULL, NULL, JOIN);
	}
}

void	kill_other_philos(t_data *philo_data, unsigned int idx)
{
	unsigned int	i;
	t_philo			*philo_ptr;
	unsigned long	time_died;

	i = 0;
	while (i < philo_data->philos_nbr)
	{
		philo_ptr = &(philo_data->philos[idx]);
		safe_mutex_handle(&philo_ptr->lock_for_dead, LOCK);
		if (philo_ptr->dead == TRUE)
		{
			time_died = philo_ptr->deadline;
			safe_mutex_handle(&philo_ptr->lock_for_dead, UNLOCK);
			time_print_helper(philo_data, time_died, idx);
		}
		else
		{
			philo_ptr->dead = TRUE;
			safe_mutex_handle(&philo_ptr->lock_for_dead, UNLOCK);
		}
		idx = (idx + 1) % philo_data->philos_nbr;
		i++;
	}
	ft_join_threads(philo_data);
}
