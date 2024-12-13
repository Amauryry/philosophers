/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activities_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberion <aberion@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 14:12:15 by aberion           #+#    #+#             */
/*   Updated: 2024/12/13 17:40:22 by aberion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	check_meal(t_data *data)
{
	int	i;

	pthread_mutex_lock(&data->meal_mutex); // Protection globale
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

// int	check_meal(t_data *data)
// {
// 	int	i;

// 	i = 0;
// 	if (!data->meal_nb)
// 		return (0);
// 	while (i < data->nb_p && data->eaten_enough[i] == 1)
// 		i++;
// 	if (i == data->nb_p)
// 	{
// 		return (42);
// 	}
// 	return (0);
// }

void	set_meal(t_data *data, t_philosopher *philo)
{
	pthread_mutex_lock(&data->meal_mutex); // Protection globale
	data->eaten_enough[philo->id] = 1;
	pthread_mutex_unlock(&data->meal_mutex);
}

// void	set_meal(t_data *data, t_philosopher *philo)
// {
// 	pthread_mutex_lock(&data->eaten_mutex[philo->id]);
// 	data->eaten_enough[philo->id] = 1;
// 	pthread_mutex_unlock(&data->eaten_mutex[philo->id]);
// }

int	handle_meal_check(t_data *data, t_philosopher *philo)
{
	if (check_meal(data))
	{
		pthread_mutex_lock(&data->meal_c_mutex);
		if (!data->meal_checker)
		{
			print_message("All the philosophers have eaten enough.", data,
				philo);
			data->meal_checker = 1;
		}
		pthread_mutex_unlock(&data->meal_c_mutex);
		return (1);
	}
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
