/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_clean.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberion <aberion@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 12:45:01 by aberion           #+#    #+#             */
/*   Updated: 2024/12/16 16:30:08 by aberion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	initialize_mutexes_and_arrays(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_p)
	{
		data->eaten_enough[i] = 0;
		data->fork_status[i] = 0;
		pthread_mutex_init(&data->forks[i], NULL);
		pthread_mutex_init(&data->fork_status_mutex[i], NULL);
		pthread_mutex_init(&data->eaten_mutex[i], NULL);
		i++;
	}
	pthread_mutex_init(&data->death_mutex, NULL);
	pthread_mutex_init(&data->print_mutex, NULL);
	pthread_mutex_init(&data->meal_mutex, NULL);
}

void	setup_data(t_data *data, char **argv)
{
	data->statring_time = get_current_time_in_ms();
	data->nb_p = ft_atoi(argv[1]);
	data->die_time = ft_atoi(argv[2]);
	data->eat_time = ft_atoi(argv[3]);
	data->sleep_time = ft_atoi(argv[4]);
	if (argv[5])
		data->meal_nb = ft_atoi(argv[5]);
	else
		data->meal_nb = 0;
	data->someone_died = 0;
	data->eaten_enough = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_p);
	data->eaten_enough = malloc(sizeof(int) * data->nb_p);
	data->eaten_mutex = malloc(sizeof(pthread_mutex_t) * data->nb_p);
	data->fork_status = malloc(sizeof(int) * data->nb_p);
	data->fork_status_mutex = malloc(sizeof(pthread_mutex_t) * data->nb_p);
	initialize_mutexes_and_arrays(data);
}

void	cleanup_resources(t_data *data)
{
	int	i;

	pthread_mutex_destroy(&data->print_mutex);
	i = 0;
	while (i < data->nb_p)
	{
		pthread_mutex_destroy(&data->fork_status_mutex[i]);
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	free(data->forks);
	free(data->fork_status);
	free(data->fork_status_mutex);
	free(data->eaten_enough);
	free(data->eaten_mutex);
}
