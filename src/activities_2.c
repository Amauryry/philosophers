/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activities_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberion <aberion@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 14:12:15 by aberion           #+#    #+#             */
/*   Updated: 2024/12/05 14:12:55 by aberion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int check_meal(t_data *data)
{
    int i = 0;
    if(!data->meal_nb)
        return (0);
    while (i < data->nb_p && data->eaten_enough[i] == 1)
        i++;
    if (i == data->nb_p)
    {
        return 42;
    }
    return 0;
}

void set_meal(t_data *data, t_philosopher *philo)
{
    pthread_mutex_lock(&data->eaten_mutex[philo->id]);
    data->eaten_enough[philo->id] = 1;
    pthread_mutex_unlock(&data->eaten_mutex[philo->id]);
}
