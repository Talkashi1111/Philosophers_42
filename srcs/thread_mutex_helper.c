/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_mutex_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 17:39:47 by tkashi            #+#    #+#             */
/*   Updated: 2024/05/11 17:48:51 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	handle_mutex_error(int status, t_operation code)
{
	if (0 == status)
		return (OK);
	if (EINVAL == status && (LOCK == code || UNLOCK == code))
		return (ft_error("error: the value specified by mutex is invalid",
				EINVAL));
	else if (EINVAL == status && INIT == code)
		return (ft_error("error: init mutex, invalid arguments", EINVAL));
	else if (EDEADLK == status)
		return (ft_error("error: deadlock would occur if the "
				"thread blocked waiting for mutex.", EDEADLK));
	else if (ENOMEM == status)
		return (ft_error("error: allocating memory failure", ENOMEM));
	else if (EBUSY == status)
		return (ft_error("error: mutex is locked", EBUSY));
	else
		return (ft_error("error: unknown error", status));
}

int	safe_mutex_handle(t_mtx *mutex, t_operation code)
{
	if (LOCK == code)
		return (handle_mutex_error(pthread_mutex_lock(mutex), code) != OK);
	else if (UNLOCK == code)
		return (handle_mutex_error(pthread_mutex_unlock(mutex), code) != OK);
	else if (INIT == code)
		return (handle_mutex_error(pthread_mutex_init(mutex, NULL),
				code) != OK);
	else if (DESTROY == code)
		return (handle_mutex_error(pthread_mutex_destroy(mutex), code) != OK);
	else
		return (ft_error("error: wrong code for mutex_handle", INTERNAL_ERROR));
}

int	handle_thread_error(int status, t_operation code)
{
	if (0 == status)
		return (OK);
	if (EINVAL == status && JOIN == code)
		return (ft_error("error: thread is not a joinable thread", EINVAL));
	else if (EINVAL == status && DETACH == code)
		return (ft_error("error: thread is not a joinable thread", EINVAL));
	else if (EINVAL == status && INIT == code)
		return (ft_error("error: invalid attributes", EINVAL));
	else if (EINVAL == status && DETACH == code)
		return (ft_error("error: thread is not a joinable thread", EINVAL));
	else if (EINVAL == status && JOIN == code)
		return (ft_error("error: thread is not a joinable thread", EINVAL));
	else if (ENOMEM == status)
		return (ft_error("error: insufficient memory to create another thread",
				ENOMEM));
	else
		return (ft_error("error: unknown error", status));
}

int	safe_thread_handle(pthread_t *thread, void *(*start_routine)(void *),
	void *data, t_operation code)
{
	if (JOIN == code)
		return (handle_thread_error(pthread_join(*thread, NULL), code) != OK);
	else if (DETACH == code)
		return (handle_thread_error(pthread_detach(*thread), code) != OK);
	else if (INIT == code)
		return (handle_thread_error(pthread_create(thread, NULL, start_routine,
					data), code) != OK);
	else
		return (ft_error("error: wrong code for thread_handle",
				INTERNAL_ERROR));
}
