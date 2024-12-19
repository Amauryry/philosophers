/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subsidiary_f.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberion <aberion@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 10:52:51 by aberion           #+#    #+#             */
/*   Updated: 2024/12/19 16:40:41 by aberion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	print_message(const char *s, t_data *data, t_philosopher *philo)
{
	pthread_mutex_lock(&data->print_mutex);
	printf("%ld %d %s\n", (get_current_time_in_ms() - data->statring_time),
		philo->id, s);
	pthread_mutex_unlock(&data->print_mutex);
}

int	ft_atoi(const char *str)
{
	int				minus;
	unsigned int	result;
	int				i;

	i = 0;
	result = 0;
	minus = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
	{
		i++;
		minus *= (-1);
	}
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		result *= 10;
		result += str[i] - 48;
		i++;
	}
	return (result *= minus);
}

long	get_current_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	check_death_or_meal(t_data *data)
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

int	skip_time(int time, t_data *data, t_philosopher *philo)
{
	long	start_time;
	long	end_time;

	(void)philo;
	start_time = get_current_time_in_ms();
	end_time = start_time + time;
	while (1)
	{
		if (is_someone_dead(data))
			return (1);
		if (get_current_time_in_ms() >= end_time)
			break ;
		usleep(1000);
	}
	return (0);
}
