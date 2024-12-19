/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activities_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberion <aberion@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 14:12:15 by aberion           #+#    #+#             */
/*   Updated: 2024/12/19 16:40:16 by aberion          ###   ########.fr       */
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
		if (check_death_or_meal(data))
			return (1);
		if (get_current_time_in_ms() >= end_time)
			break ;
		usleep(1000);
	}
	return (0);
}
