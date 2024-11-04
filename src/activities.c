/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activities.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberion <aberion@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:37:38 by aberion           #+#    #+#             */
/*   Updated: 2024/10/30 17:24:38 by aberion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"
#include <pthread.h>

int check_meal(t_data *data)
{
    int i = 0;
    while (i < data->nb_p && data->eaten_enough[i] == 1)
        i++;
    if (i == data->nb_p)
        return 42;
    return 0;
}

void set_meal(t_data *data, t_philosopher *philo)
{
    pthread_mutex_lock(&data->eaten_mutex[philo->id]);
    data->eaten_enough[philo->id] = 1;
    pthread_mutex_unlock(&data->eaten_mutex[philo->id]);
}

int     eat_routine(t_data *data, t_philosopher *philo)
{
    int temp;
    
    lock_forks(data, philo->id);
    pthread_mutex_lock(&data->print_mutex);
    if (!is_someone_dead(data))
    {
        printf("%ld %d has taken a fork\n", (get_current_time_in_ms() - data->statring_time), philo->id);
        printf("%ld %d is eating\n", (get_current_time_in_ms() - data->statring_time), philo->id);
    }
    pthread_mutex_unlock(&data->print_mutex);
    temp = skip_time(data->eat_time, data, philo);
    if (temp == 42)
    {  
        unlock_forks(data, philo->id);
        man_down(data, philo);
        return (42);
    }
    if (temp == 1)
    {
        unlock_forks(data, philo->id);
        return 1;
    }
    philo->last_meal_time = get_current_time_in_ms();
    philo->meal_count++;
    if(philo->meal_count == data->meal_nb)
        set_meal(data, philo);
    unlock_forks(data, philo->id);
    if (check_meal(data))
    {
        printf("%d === %d\n", philo->id, philo->meal_count);
        exit(0);
    }
    return 0;
}

int sleep_routine(t_data *data, t_philosopher *philo)
{
    int temp;
    
    pthread_mutex_lock(&data->print_mutex);
    if (!is_someone_dead(data))
    {
        printf("%ld %d is sleeping\n", (get_current_time_in_ms() - data->statring_time), philo->id);
    }
    pthread_mutex_unlock(&data->print_mutex);
    temp = skip_time(data->sleep_time, data, philo);
    if (temp == 42)
    {  
        man_down(data, philo);
        return 42;
    }
    if (temp == 1)
        return 1;
    return 0;
}

int waiting_room(t_data *data, t_philosopher *philo)
{
    int left_fork = philo->id;
    int right_fork = (philo->id + 1) % data->nb_p;
    
    while ((get_current_time_in_ms() - philo->last_meal_time) < data->die_time)
    {
        pthread_mutex_lock(&data->death_mutex);
        // printf("id == %d\n", philo->id);
        if (data->someone_died)
        {
            pthread_mutex_unlock(&data->death_mutex);
            return 1;
        }
        pthread_mutex_unlock(&data->death_mutex);
        pthread_mutex_lock(&data->fork_status_mutex[left_fork]);
        pthread_mutex_lock(&data->fork_status_mutex[right_fork]);
        if (data->fork_status[left_fork] == 0 && data->fork_status[right_fork] == 0)
        {   
            pthread_mutex_unlock(&data->fork_status_mutex[left_fork]);
            pthread_mutex_unlock(&data->fork_status_mutex[right_fork]);
            return 0;
        }
        pthread_mutex_unlock(&data->fork_status_mutex[left_fork]);
        pthread_mutex_unlock(&data->fork_status_mutex[right_fork]);
    }
    man_down(data, philo);
    return 42;
}

