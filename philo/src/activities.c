/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activities.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberion <aberion@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:37:38 by aberion           #+#    #+#             */
/*   Updated: 2024/12/18 15:24:53 by aberion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"
#include <pthread.h>
#include <unistd.h>

int	eat_routine(t_data *data, t_philosopher *philo)
{
	int	temp;

	if(lock_forks(data, philo))
		return 1;
	if (!is_someone_dead(data))
	{
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
		// man_down(data, philo);
		return (42);
	}
	if (temp == 1)
		return (1);
	return (0);
}

int	think_routine(t_data *data, t_philosopher *philo)
{
	if (is_someone_dead(data))
		return (1);
	print_message("is thinking", data, philo);
	usleep(1000);
	return (0);
}
