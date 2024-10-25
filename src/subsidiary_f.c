/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subsidiary_f.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberion <aberion@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-10-25 10:52:51 by aberion           #+#    #+#             */
/*   Updated: 2024-10-25 10:52:51 by aberion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"
#include <unistd.h>

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

long get_current_time_in_ms()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

int skip_time(int time, t_data *data, t_philosopher *philo)
{
    long start_time = get_current_time_in_ms();
    long end_time = start_time + time;

    while (get_current_time_in_ms() < end_time)
    {
        usleep(1000);  // Sleep for 1 millisecond to avoid busy-waiting
        pthread_mutex_lock(&data->death_mutex);
        if (data->someone_died || (get_current_time_in_ms() - philo->last_meal_time) > data->die_time)
        {
			printf("test\n");
			data->someone_died = 1;
            pthread_mutex_unlock(&data->death_mutex);
            return 1;
        }
        pthread_mutex_unlock(&data->death_mutex);
    }
    return 0;
}

void man_down(t_philosopher *philo)
{
    long current_time = get_current_time_in_ms();
    pthread_mutex_lock(&philo->data->death_mutex);
    if (!philo->data->someone_died)
    {
        philo->data->someone_died = 1;  // Marque un philosophe comme mort
        printf("%ld %d died\n", current_time, philo->id);
    }
    pthread_mutex_unlock(&philo->data->death_mutex);
}
