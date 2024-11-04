/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 21:52:32 by tkashi            #+#    #+#             */
/*   Updated: 2024/05/13 14:42:44 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	alloc_data(t_data *philo_data)
{
	unsigned int	i;

	philo_data->philos = ft_calloc(sizeof(t_philo), philo_data->philos_nbr);
	if (!philo_data->philos)
		return (ft_error("error: failed alloc philos", ALLOC_ERROR));
	philo_data->forks = ft_calloc(sizeof(t_mtx), philo_data->philos_nbr);
	if (!philo_data->forks)
	{
		free(philo_data->philos);
		philo_data->philos = NULL;
		return (ft_error("error: failed alloc forks", ALLOC_ERROR));
	}
	i = 0;
	while (i < philo_data->philos_nbr)
	{
		philo_data->philos[i].idx = i;
		philo_data->philos[i].meal_counter = 0;
		philo_data->philos[i].dead = FALSE;
		philo_data->philos[i].data = philo_data;
		i++;
	}
	return (OK);
}

int	init_fork_mutexes(t_data *philo_data)
{
	unsigned int	i;
	int				res;

	i = 0;
	while (i < philo_data->philos_nbr)
	{
		res = safe_mutex_handle(&philo_data->forks[i], INIT);
		if (res != OK)
		{
			while (i > 0)
			{
				i--;
				safe_mutex_handle(&philo_data->forks[i], DESTROY);
			}
			return (res);
		}
		i++;
	}
	return (OK);
}

int	init_philo_mutexes(t_philo *philo)
{
	int	res;

	res = safe_mutex_handle(&philo->lock_meal_counter, INIT);
	if (res != OK)
		return (res);
	res = safe_mutex_handle(&philo->lock_for_dead, INIT);
	if (res != OK)
	{
		safe_mutex_handle(&philo->lock_meal_counter, DESTROY);
		return (res);
	}
	return (OK);
}

int	init_mutexes(t_data *philo_data)
{
	unsigned int	i;
	int				res;

	res = safe_mutex_handle(&philo_data->lock_print, INIT);
	if (res != OK)
		return (res);
	res = init_fork_mutexes(philo_data);
	if (res != OK)
		return (safe_mutex_handle(&philo_data->lock_print, DESTROY));
	i = 0;
	while (i < philo_data->philos_nbr)
	{
		res = init_philo_mutexes(&philo_data->philos[i]);
		if (res != OK)
			return (reverse_mutex_destroy(philo_data, i, res));
		i++;
	}
	return (OK);
}

int	init_data(t_data *philo_data, int argc, char *argv[])
{
	if (atou(argv[1], &philo_data->philos_nbr) != OK
		|| philo_data->philos_nbr < 1)
		return (ft_error("error: number_of_philosophers invalid", ARG_ERROR));
	if (atou(argv[2], &philo_data->time2die) != OK)
		return (ft_error("error: time_to_die invalid", ARG_ERROR));
	if (atou(argv[3], &philo_data->time2eat) != OK)
		return (ft_error("error: time_to_eat invalid", ARG_ERROR));
	if (atou(argv[4], &philo_data->time2sleep) != OK)
		return (ft_error("error: time_to_sleep invalid", ARG_ERROR));
	philo_data->meals_present = FALSE;
	if (argc == 6)
	{
		if (atou(argv[5], &philo_data->meals_nbr) != OK)
			return (ft_error("error: number_of_times_each_philosopher_"
					"must_eat invalid", ARG_ERROR));
		philo_data->meals_present = TRUE;
	}
	if (alloc_data(philo_data) != OK)
		return (ALLOC_ERROR);
	return (OK);
}
