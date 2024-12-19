/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_clean.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberion <aberion@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 12:45:01 by aberion           #+#    #+#             */
/*   Updated: 2024/12/19 15:25:01 by aberion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	initialize_mutexes_and_arrays(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_p)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&data->death_mutex, NULL);
	pthread_mutex_init(&data->last_meal_mutex, NULL);
	pthread_mutex_init(&data->meal_c_mutex, NULL);
	pthread_mutex_init(&data->print_mutex, NULL);
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
	initialize_mutexes_and_arrays(data);
}

void	cleanup_resources(t_data *data)
{
	int	i;

	pthread_mutex_destroy(&data->death_mutex);
	pthread_mutex_destroy(&data->last_meal_mutex);
	pthread_mutex_destroy(&data->meal_c_mutex);
	pthread_mutex_destroy(&data->print_mutex);
	i = 0;
	while (i < data->nb_p)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	free(data->forks);
}

void error_cleanup(t_data *data, t_philosopher *philo_args, pthread_t *philosophers, int i)
{
	int j;

	j = 0;
	while (j < i)
	{
		pthread_join(philosophers[j], NULL);
		pthread_mutex_destroy(&data->forks[j]);
		j++;
	}
	pthread_mutex_destroy(&data->death_mutex);
	pthread_mutex_destroy(&data->last_meal_mutex);
	pthread_mutex_destroy(&data->meal_c_mutex);
	pthread_mutex_destroy(&data->print_mutex);
	free(data->forks);
	free(philo_args);
	free(philosophers);
}
