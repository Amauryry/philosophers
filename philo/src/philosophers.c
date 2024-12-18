/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberion <aberion@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 00:27:03 by aberion           #+#    #+#             */
/*   Updated: 2024/12/16 17:57:39 by aberion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"
#include <stdbool.h>
#include <unistd.h>

// TIME TO DIE == TIME TO EAT == TIME TO SLEEP
void	*philosopher(void *arg)
{
	t_philosopher	*philo;
	t_data			*data;

	philo = (t_philosopher *)arg;
	data = philo->data;
	while (1)
	{
		if (is_someone_dead(data))
			break ;
		if (waiting_room(data, philo))
			break ;
		if (eat_routine(data, philo))
			break ;
		if (sleep_routine(data, philo))
			break ;
		if (think_routine(data, philo))
			break ;
	}
	return (NULL);
}

void	initialize_philosophers(t_philosopher *philo_args,
		pthread_t *philosophers, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_p)
	{
		philo_args[i].meal_nb = data->meal_nb;
		philo_args[i].id = i;
		philo_args[i].meal_count = 0;
		philo_args[i].data = data;
		philo_args[i].last_meal_time = get_current_time_in_ms();
		pthread_create(&philosophers[i], NULL, philosopher, &philo_args[i]);
		i++;
	}
}

void	manage_philosophers(t_data *data)
{
	pthread_t		*philosophers;
	t_philosopher	*philo_args;
	int				i;

	philosophers = malloc(sizeof(pthread_t) * data->nb_p);
	if (!philosophers)
		return ;
	philo_args = malloc(sizeof(t_philosopher) * data->nb_p);
	if (!philo_args)
	{
		free(philosophers);
		return ;
	}
	i = 0;
	initialize_philosophers(philo_args, philosophers, data);
	i = 0;
	while (i < data->nb_p)
		pthread_join(philosophers[i++], NULL);
	cleanup_resources(data);
	free(philosophers);
	free(philo_args);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (launch_parsing(argc, argv))
	{
		printf("error\n");
		return (1);
	}
	setup_data(&data, argv);
	if (data.nb_p == 1)
	{
		usleep(1000 * data.die_time);
		printf("%ld 0 has died.\n", get_current_time_in_ms()
			- data.statring_time);
		free(data.forks);
		return (0);
	}
	manage_philosophers(&data);
	return (0);
}
