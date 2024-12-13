/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sub_f_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberion <aberion@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 13:08:50 by aberion           #+#    #+#             */
/*   Updated: 2024/12/13 16:38:56 by aberion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	man_down(t_data *data, t_philosopher *philo)
{
	pthread_mutex_lock(&philo->data->death_mutex);
	philo->data->someone_died = 1;
	pthread_mutex_unlock(&philo->data->death_mutex);
	pthread_mutex_lock(&data->print_mutex);
	if (!data->meal_checker)
		printf("%ld %d died\n", (get_current_time_in_ms()
				- philo->data->statring_time), philo->id);
	pthread_mutex_unlock(&data->print_mutex);
}

void	set_fork_status(t_data *data, int id, int status)
{
	pthread_mutex_lock(&data->fork_status_mutex[id]);
	data->fork_status[id] = status;
	pthread_mutex_unlock(&data->fork_status_mutex[id]);
}


void lock_forks(t_data *data, int id)
{
    int left_fork = id;
    int right_fork = (id + 1) % data->nb_p;

    if (left_fork > right_fork) // Garantir l'ordre
    {
        int temp = left_fork;
        left_fork = right_fork;
        right_fork = temp;
    }

    pthread_mutex_lock(&data->forks[left_fork]);
 	set_fork_status(data, left_fork, 1);
    pthread_mutex_lock(&data->forks[right_fork]);
 	set_fork_status(data, right_fork, 1);
}

void unlock_forks(t_data *data, int id)
{
    int left_fork = id;
    int right_fork = (id + 1) % data->nb_p;

    if (left_fork > right_fork) // Garantir l'ordre
    {
        int temp = left_fork;
        left_fork = right_fork;
        right_fork = temp;
    }

    pthread_mutex_unlock(&data->forks[right_fork]);
 	set_fork_status(data, right_fork, 0);
    pthread_mutex_unlock(&data->forks[left_fork]);
 	set_fork_status(data, left_fork, 0);
}

// void	lock_forks(t_data *data, int id)
// {
// 	if (id % 2 == 0)
// 	{
// 		pthread_mutex_lock(&data->forks[id]);
// 		set_fork_status(data, id, 1);
// 		pthread_mutex_lock(&data->forks[(id + 1) % data->nb_p]);
// 		set_fork_status(data, (id + 1) % data->nb_p, 1);
// 	}
// 	else
// 	{
// 		pthread_mutex_lock(&data->forks[(id + 1) % data->nb_p]);
// 		set_fork_status(data, (id + 1) % data->nb_p, 1);
// 		pthread_mutex_lock(&data->forks[id]);
// 		set_fork_status(data, id, 1);
// 	}
// }

// void	unlock_forks(t_data *data, int id)
// {
// 	if (id % 2 == 0)
// 	{
// 		set_fork_status(data, id, 0);
// 		pthread_mutex_unlock(&data->forks[id]);
// 		set_fork_status(data, (id + 1) % data->nb_p, 0);
// 		pthread_mutex_unlock(&data->forks[(id + 1) % data->nb_p]);
// 	}
// 	else
// 	{
// 		set_fork_status(data, (id + 1) % data->nb_p, 0);
// 		pthread_mutex_unlock(&data->forks[(id + 1) % data->nb_p]);
// 		set_fork_status(data, id, 0);
// 		pthread_mutex_unlock(&data->forks[id]);
// 	}
// }

int	is_someone_dead(t_data *data)
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
