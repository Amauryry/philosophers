/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activities.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberion <aberion@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:37:38 by aberion           #+#    #+#             */
/*   Updated: 2024/10/29 17:20:04 by aberion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int     eat_routine(t_data *data, t_philosopher *philo)
{
    long current_time;
    int id = philo->id;
    int temp;

    lock_forks(data, id);
    current_time = get_current_time_in_ms();
    pthread_mutex_lock(&data->print_mutex);
    if (!is_someone_dead(data))
    {
        printf("%ld %d has taken a fork\n", current_time, id);
        printf("%ld %d is eating\n", current_time, id);
    }
    pthread_mutex_unlock(&data->print_mutex);
    philo->last_meal_time = current_time;
    temp = skip_time(data->eat_time, data, philo);
    if (temp == 42)
    {  
        unlock_forks(data, id);
        man_down(philo);
        return (42);
    }
    if (temp == 1)
    {
         unlock_forks(data, id);
        return 1;
    }
    unlock_forks(data, id);
    return 0;
}

int sleep_routine(t_data *data, t_philosopher *philo)
{
    long current_time;
    int id = philo->id;
    int temp;
    
    current_time = get_current_time_in_ms();
    pthread_mutex_lock(&data->print_mutex);
    printf("%ld %d has taken a fork\n", current_time, id);
    printf("%ld %d is eating\n", current_time, id);
    pthread_mutex_unlock(&data->print_mutex);
    temp = skip_time(data->sleep_time, data, philo);
    if (temp == 42)
    {  
        man_down(philo);
        return 42;
    }
    if (temp == 1)
        return 1;
    return 0;
}
