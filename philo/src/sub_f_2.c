/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sub_f_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberion <aberion@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 13:08:50 by aberion           #+#    #+#             */
/*   Updated: 2024/12/18 15:22:28 by aberion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"
#include <pthread.h>

void	man_down(t_data *data, t_philosopher *philo)
{
	pthread_mutex_lock(&philo->data->death_mutex);
	pthread_mutex_lock(&data->print_mutex);
	if (!philo->data->someone_died)
	{
		printf("%ld %d died\n", (get_current_time_in_ms()
				- philo->data->statring_time), philo->id);
		philo->data->someone_died = 1;
	}
	pthread_mutex_unlock(&data->print_mutex);
	pthread_mutex_unlock(&philo->data->death_mutex);
}

int	lock_forks(t_data *data, t_philosopher *philo)
{
	int	left_fork;
	int	right_fork;
	int	temp;

	left_fork = philo->id;
	right_fork = (philo->id + 1) % data->nb_p;
	if (left_fork > right_fork)
	{
		temp = left_fork;
		left_fork = right_fork;
		right_fork = temp;
	}
	pthread_mutex_lock(&data->forks[left_fork]);
	if (is_someone_dead(data))
	{
		pthread_mutex_unlock(&data->forks[left_fork]);
		return 1;
	}
	print_message("has taken a fork", data, philo);
	pthread_mutex_lock(&data->forks[right_fork]);
	if (is_someone_dead(data))
	{
		pthread_mutex_unlock(&data->forks[right_fork]);
		pthread_mutex_unlock(&data->forks[left_fork]);
		return 1;
	}
	print_message("has taken a fork", data, philo);
	return 0;
}

void	unlock_forks(t_data *data, int id)
{
	int	left_fork;
	int	right_fork;
	int	temp;

	left_fork = id;
	right_fork = (id + 1) % data->nb_p;
	if (left_fork > right_fork)
	{
		temp = left_fork;
		left_fork = right_fork;
		right_fork = temp;
	}
	pthread_mutex_unlock(&data->forks[right_fork]);
	// set_fork_status(data, right_fork, 0);
	pthread_mutex_unlock(&data->forks[left_fork]);
	// set_fork_status(data, left_fork, 0);
}

int	is_someone_dead(t_data *data)
{
	pthread_mutex_lock(&data->death_mutex);
	if (data->someone_died)
	{
		pthread_mutex_unlock(&data->death_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->death_mutex);
	return (0);
}
