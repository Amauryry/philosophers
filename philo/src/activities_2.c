/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activities_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberion <aberion@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 14:12:15 by aberion           #+#    #+#             */
/*   Updated: 2024/12/16 16:28:31 by aberion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	check_meal(t_data *data)
{
	int	i;

	pthread_mutex_lock(&data->meal_mutex);
	i = 0;
	if (!data->meal_nb)
	{
		pthread_mutex_unlock(&data->meal_mutex);
		return (0);
	}
	while (i < data->nb_p && data->eaten_enough[i] == 1)
		i++;
	if (i == data->nb_p)
	{
		pthread_mutex_unlock(&data->meal_mutex);
		return (42);
	}
	pthread_mutex_unlock(&data->meal_mutex);
	return (0);
}

int	handle_meal_state(t_data *data, t_philosopher *philo, int temp)
{
	if (temp)
	{
		if (temp == 42)
			man_down(data, philo);
		return (1);
	}
	philo->last_meal_time = get_current_time_in_ms();
	philo->meal_count++;
	return (0);
}

int	skip_time_eating(int time, t_data *data)
{
	long	start_time;
	long	end_time;

	start_time = get_current_time_in_ms();
	end_time = start_time + time;
	while (1)
	{
		pthread_mutex_lock(&data->death_mutex);
		if (check_death_or_meal(data))
			return (1);
		pthread_mutex_unlock(&data->death_mutex);
		if (get_current_time_in_ms() >= end_time)
			break ;
		usleep(1000);
	}
	return (0);
}
