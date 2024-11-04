/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_routines.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 21:57:18 by tkashi            #+#    #+#             */
/*   Updated: 2024/05/13 14:50:10 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_action(t_philo *philo, t_action action)
{
	unsigned long	now;

	if (current_time_ms(&now) != OK)
		return ;
	safe_mutex_handle(&philo->data->lock_print, LOCK);
	if (action == FORK)
		printf("%lu %u has taken a fork\n", now, philo->idx + 1);
	else if (action == EAT)
		printf("%lu %u is eating\n", now, philo->idx + 1);
	else if (action == SLEEP)
		printf("%lu %u is sleeping\n", now, philo->idx + 1);
	else if (action == THINK)
		printf("%lu %u is thinking\n", now, philo->idx + 1);
	safe_mutex_handle(&philo->data->lock_print, UNLOCK);
}

int	is_dead(t_philo *philo)
{
	unsigned long	now;

	if (current_time_ms(&now) != OK)
		return (FALSE);
	safe_mutex_handle(&philo->lock_for_dead, LOCK);
	if (philo->dead == TRUE)
	{
		safe_mutex_handle(&philo->lock_for_dead, UNLOCK);
		return (TRUE);
	}
	if (now >= philo->deadline)
	{
		philo->deadline = now;
		philo->dead = TRUE;
		safe_mutex_handle(&philo->lock_for_dead, UNLOCK);
		return (TRUE);
	}
	safe_mutex_handle(&philo->lock_for_dead, UNLOCK);
	return (FALSE);
}

int	update_deadline(t_philo *philo)
{
	unsigned long	now;

	if (current_time_ms(&now) != OK)
	{
		safe_mutex_handle(&philo->lock_for_dead, LOCK);
		philo->dead = TRUE;
		safe_mutex_handle(&philo->lock_for_dead, UNLOCK);
		return (DIED_ERROR);
	}
	safe_mutex_handle(&philo->lock_for_dead, LOCK);
	philo->deadline = now + philo->data->time2die;
	safe_mutex_handle(&philo->lock_for_dead, UNLOCK);
	return (OK);
}

int	go_sleep(t_philo *philo)
{
	if (is_dead(philo) == TRUE)
		return (DIED_ERROR);
	print_action(philo, SLEEP);
	ft_msleep(philo->data->time2sleep);
	return (OK);
}

int	reverse_mutex_destroy(t_data *philo_data, unsigned int i, int res)
{
	while (i > 0)
	{
		i--;
		safe_mutex_handle(&philo_data->philos[i].lock_meal_counter,
			DESTROY);
		safe_mutex_handle(&philo_data->philos[i].lock_for_dead,
			DESTROY);
	}
	destroy_fork_mutexes(philo_data);
	safe_mutex_handle(&philo_data->lock_print, DESTROY);
	return (res);
}
