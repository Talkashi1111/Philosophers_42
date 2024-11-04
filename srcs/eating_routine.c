/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 20:47:21 by tkashi            #+#    #+#             */
/*   Updated: 2024/05/13 18:31:31 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	leave_forks(t_philo *philo)
{
	unsigned int	right_fork;
	unsigned int	left_fork;

	left_fork = philo->idx;
	right_fork = (philo->idx + 1) % philo->data->philos_nbr;
	if ((philo->idx % 2) == 0)
	{
		safe_mutex_handle(&philo->data->forks[right_fork], UNLOCK);
		safe_mutex_handle(&philo->data->forks[left_fork], UNLOCK);
	}
	else
	{
		safe_mutex_handle(&philo->data->forks[left_fork], UNLOCK);
		safe_mutex_handle(&philo->data->forks[right_fork], UNLOCK);
	}
}

int	eat(t_philo *philo)
{
	if (is_dead(philo) == TRUE)
		return (DIED_ERROR);
	if (take_forks(philo) != OK)
		return (DIED_ERROR);
	print_action(philo, EAT);
	ft_msleep(philo->data->time2eat);
	leave_forks(philo);
	if (update_deadline(philo) != OK)
		return (DIED_ERROR);
	safe_mutex_handle(&philo->lock_meal_counter, LOCK);
	if (philo->data->meals_present)
	{
		if (philo->meal_counter < philo->data->meals_nbr)
			philo->meal_counter++;
	}
	safe_mutex_handle(&philo->lock_meal_counter, UNLOCK);
	return (OK);
}

int	take_forks_helper(t_philo *philo, unsigned int first_fork,
	unsigned int second_fork)
{
	safe_mutex_handle(&philo->data->forks[first_fork], LOCK);
	if (is_dead(philo) == TRUE)
	{
		safe_mutex_handle(&philo->data->forks[first_fork], UNLOCK);
		return (DIED_ERROR);
	}
	print_action(philo, FORK);
	if (first_fork == second_fork)
	{
		while (is_dead(philo) == FALSE)
			ft_msleep(5);
		safe_mutex_handle(&philo->data->forks[first_fork], UNLOCK);
		return (DIED_ERROR);
	}
	safe_mutex_handle(&philo->data->forks[second_fork], LOCK);
	if (is_dead(philo) == TRUE)
	{
		safe_mutex_handle(&philo->data->forks[second_fork], UNLOCK);
		safe_mutex_handle(&philo->data->forks[first_fork], UNLOCK);
		return (DIED_ERROR);
	}
	print_action(philo, FORK);
	return (OK);
}

int	take_forks(t_philo *philo)
{
	unsigned int	right_fork;
	unsigned int	left_fork;

	left_fork = philo->idx;
	right_fork = (philo->idx + 1) % philo->data->philos_nbr;
	if (philo->idx % 2 == 0)
	{
		if (take_forks_helper(philo, left_fork, right_fork) != OK)
			return (DIED_ERROR);
	}
	else
	{
		if (take_forks_helper(philo, right_fork, left_fork) != OK)
			return (DIED_ERROR);
	}
	return (OK);
}
