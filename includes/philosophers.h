/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 17:36:43 by tkashi            #+#    #+#             */
/*   Updated: 2024/05/13 18:41:23 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>
# include <stdint.h>
# include <errno.h>
# include <string.h>
# include <stdbool.h>
# include <stdlib.h>

# define UINT_MAX 4294967295
# define FALSE	0
# define TRUE	1

enum {
	OK,
	ARG_ERROR,
	ALLOC_ERROR,
	INTERNAL_ERROR,
	DIED_ERROR,
};

typedef enum e_action {
	FORK,
	EAT,
	SLEEP,
	THINK,
}	t_action;

typedef enum e_operation
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	JOIN,
	DETACH,
}	t_operation;

typedef pthread_mutex_t	t_mtx;

struct					s_philo;

typedef struct s_data
{
	struct s_philo	*philos;
	t_mtx			*forks;
	t_mtx			lock_print;
	unsigned int	philos_nbr;
	unsigned int	time2die;
	unsigned int	time2eat;
	unsigned int	time2sleep;
	char			meals_present;
	unsigned int	meals_nbr;
}	t_data;

typedef struct s_philo
{
	pthread_t		thread_id;
	unsigned int	idx;
	t_mtx			lock_meal_counter;
	unsigned int	meal_counter;
	unsigned long	deadline;
	t_mtx			lock_for_dead;
	char			dead;
	t_data			*data;
}	t_philo;

int		atou(char *str, unsigned int *num);
int		ft_error(const char *msg, int rev_val);
int		current_time_ms(unsigned long *current_time);
void	*ft_calloc(size_t nmemb, size_t size);
int		safe_mutex_handle(t_mtx *mutex, t_operation code);
int		init_data(t_data *philo_data, int argc, char *argv[]);
int		init_mutexes(t_data *philo_data);
int		init_philo_mutexes(t_philo *philo);
int		init_fork_mutexes(t_data *philo_data);
int		alloc_data(t_data *philo_data);
int		safe_thread_handle(pthread_t *thread, void *(*start_routine)(void *),
			void *data, t_operation code);
int		handle_thread_error(int status, t_operation code);
int		safe_mutex_handle(t_mtx *mutex, t_operation code);
int		handle_mutex_error(int status, t_operation code);
void	free_data(t_data *philo_data);
void	destroy_mutexes(t_data *philo_data);
int		reverse_mutex_destroy(t_data *philo_data, unsigned int i, int res);
void	destroy_fork_mutexes(t_data *philo_data);
void	revert_threads(t_data *philo_data, unsigned int idx);
void	kill_other_philos(t_data *philo_data, unsigned int idx);
void	ft_join_threads(t_data *philo_data);
void	ft_supervisor(t_data *philo_data);
int		ft_msleep(long msec);
void	print_action(t_philo *philo, t_action action);
int		is_dead(t_philo *philo);
int		update_deadline(t_philo *philo);
void	time_print_helper(t_data *philo_data, unsigned long time,
			unsigned int idx);
int		go_sleep(t_philo *philo);
int		eat(t_philo *philo);
void	leave_forks(t_philo *philo);
int		take_forks(t_philo *philo);
int		take_forks_helper(t_philo *philo, unsigned int first_fork,
			unsigned int second_fork);
#endif
