/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activities_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberion <aberion@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 14:12:15 by aberion           #+#    #+#             */
/*   Updated: 2024/12/18 13:52:57 by aberion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"


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
		{
			pthread_mutex_unlock(&data->death_mutex);			
			return (1);
		}
		pthread_mutex_unlock(&data->death_mutex);
		if (get_current_time_in_ms() >= end_time)
			break ;
		usleep(1000);
	}
	return (0);
}
