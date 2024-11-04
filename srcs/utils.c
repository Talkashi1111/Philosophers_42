/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 17:42:03 by tkashi            #+#    #+#             */
/*   Updated: 2024/05/13 14:51:29 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <string.h>

int	ft_error(const char *msg, int ret_val)
{
	printf("%s\n", msg);
	return (ret_val);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*arr;

	arr = malloc(nmemb * size);
	if (!arr)
		return (NULL);
	memset(arr, 0, nmemb * size);
	return (arr);
}

int	current_time_ms(unsigned long *current_time)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != OK)
		return (ft_error("error: gettimeofday() failed", errno));
	*current_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (OK);
}

long	get_elapsed_time_microseconds(struct timeval start, struct timeval end)
{
	struct timeval	elapsed;

	elapsed.tv_sec = end.tv_sec - start.tv_sec;
	elapsed.tv_usec = end.tv_usec - start.tv_usec;
	return ((elapsed.tv_sec) * 1000000L + (elapsed.tv_usec));
}

int	ft_msleep(long msec)
{
	struct timeval	start;
	struct timeval	current;
	long			elapsed;
	long			rem;

	if (gettimeofday(&start, NULL) != OK)
		return (ft_error("error: gettimeofday() failed", errno));
	while (TRUE)
	{
		if (gettimeofday(&current, NULL) != OK)
			return (ft_error("error: gettimeofday() failed", errno));
		elapsed = get_elapsed_time_microseconds(start, current);
		rem = msec * 1000 - elapsed;
		if (rem > 1000)
			usleep(rem / 2);
		if (elapsed >= msec * 1000)
			break ;
	}
	return (OK);
}
