/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subsidiary_f.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberion <aberion@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 10:52:51 by aberion           #+#    #+#             */
/*   Updated: 2024/10/29 17:16:05 by aberion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"
#include <unistd.h>
#include <stdlib.h>

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
        if (data->someone_died)
        {
            pthread_mutex_unlock(&data->death_mutex);
            return 1;
        }
        if ((get_current_time_in_ms() - philo->last_meal_time) > data->die_time)
        {
            data->someone_died = 1;
            pthread_mutex_unlock(&data->death_mutex);
            return 42;
        }
        pthread_mutex_unlock(&data->death_mutex);
    }
    return 0;
}

void man_down(t_philosopher *philo)
{
    long current_time = get_current_time_in_ms();
    pthread_mutex_lock(&philo->data->death_mutex);
    philo->data->someone_died = 1;  // Marque un philosophe comme mort
    printf("%ld %d died\n", current_time, philo->id);
    pthread_mutex_unlock(&philo->data->death_mutex);
}

void lock_forks(t_data *data, int id)
{
    if (id % 2 == 0)
    {
        pthread_mutex_lock(&data->forks[id]);
        pthread_mutex_lock(&data->forks[(id + 1) % data->nb_p]);
    }
    else
    {
        pthread_mutex_lock(&data->forks[(id + 1) % data->nb_p]);
        pthread_mutex_lock(&data->forks[id]);
    }
}

void unlock_forks(t_data *data, int id)
{
    pthread_mutex_unlock(&data->forks[id]);
    pthread_mutex_unlock(&data->forks[(id + 1) % data->nb_p]);
}

int is_someone_dead(t_data *data)
{
    pthread_mutex_lock(&data->death_mutex);
        if (data->someone_died)
        {
            pthread_mutex_unlock(&data->death_mutex);
            return 1;
        }
    pthread_mutex_unlock(&data->death_mutex);
    return 0;
}
