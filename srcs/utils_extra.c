/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_extra.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 18:39:50 by tkashi            #+#    #+#             */
/*   Updated: 2024/05/13 18:40:03 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	time_print_helper(t_data *philo_data, unsigned long time,
	unsigned int idx)
{
	safe_mutex_handle(&philo_data->lock_print, LOCK);
	printf("%lu %u died\n", time, idx + 1);
	safe_mutex_handle(&philo_data->lock_print, UNLOCK);
}
