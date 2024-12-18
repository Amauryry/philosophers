/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activities.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberion <aberion@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:37:38 by aberion           #+#    #+#             */
/*   Updated: 2024/12/17 13:14:20 by aberion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"
#include <pthread.h>
#include <unistd.h>

int	eat_routine(t_data *data, t_philosopher *philo)
{
	int	temp;

	lock_forks(data, philo->id);
	if (!is_someone_dead(data))
	{
		print_message("has taken a fork", data, philo);
		print_message("is eating", data, philo);
		philo->last_meal_time = get_current_time_in_ms();
		philo->meal_count++;
	}
	temp = skip_time_eating(data->eat_time, data);
	if (philo->meal_count == philo->meal_nb)
	{
		unlock_forks(data, philo->id);
		return (1);
	}
	unlock_forks(data, philo->id);
	return (0);
}

int	sleep_routine(t_data *data, t_philosopher *philo)
{
	int	temp;

	if (!is_someone_dead(data))
	{
		pthread_mutex_lock(&data->print_mutex);
		printf("%ld %d is sleeping\n", (get_current_time_in_ms()
				- data->statring_time), philo->id);
		pthread_mutex_unlock(&data->print_mutex);
	}
	temp = skip_time(data->sleep_time, data, philo);
	if (temp == 42)
	{
		man_down(data, philo);
		return (42);
	}
	if (temp == 1)
		return (1);
	return (0);
}

int	check_forks_status(t_data *data, int left_fork, int right_fork)
{
	int	temp;

	if (left_fork > right_fork)
	{
		temp = left_fork;
		left_fork = right_fork;
		right_fork = temp;
	}
	pthread_mutex_lock(&data->fork_status_mutex[left_fork]);
	pthread_mutex_lock(&data->fork_status_mutex[right_fork]);
	if (data->fork_status[left_fork] == 0 && data->fork_status[right_fork] == 0)
	{
		pthread_mutex_unlock(&data->fork_status_mutex[right_fork]);
		pthread_mutex_unlock(&data->fork_status_mutex[left_fork]);
		return (0);
	}
	pthread_mutex_unlock(&data->fork_status_mutex[right_fork]);
	pthread_mutex_unlock(&data->fork_status_mutex[left_fork]);
	return (1);
}

int	waiting_room(t_data *data, t_philosopher *philo)
{
	int	left_fork;
	int	right_fork;

	left_fork = philo->id;
	right_fork = (philo->id + 1) % data->nb_p;
	if (philo->id % 2 != 0)
		usleep(2000);
	while ((get_current_time_in_ms() - philo->last_meal_time) < data->die_time)
	{
		pthread_mutex_lock(&data->death_mutex);
		if (data->someone_died)
		{
			pthread_mutex_unlock(&data->death_mutex);
			return (1);
		}
		pthread_mutex_unlock(&data->death_mutex);
		if (check_forks_status(data, left_fork, right_fork) == 0)
			return (0);
	}
	man_down(data, philo);
	return (42);
}

int	think_routine(t_data *data, t_philosopher *philo)
{
	if (is_someone_dead(data))
		return (1);
	print_message("is thinking", data, philo);
	return (0);
}
