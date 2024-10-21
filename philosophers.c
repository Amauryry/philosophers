/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberion <aberion@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 00:27:03 by aberion           #+#    #+#             */
/*   Updated: 2024/10/21 11:25:31 by aberion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
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

// Utility to get the current time in milliseconds
long get_current_time_in_ms()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

// Function that simulates the philosopher's actions
void *philosopher(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    int id = philo->id;
    t_data *data = philo->data;
    long current_time;

    while (1) {
        // Check if someone already died
        pthread_mutex_lock(&data->death_mutex);
        if (data->someone_died)
        {
            pthread_mutex_unlock(&data->death_mutex);
            break;
        }
        pthread_mutex_unlock(&data->death_mutex);

        if (!data->someone_died)
        {
            pthread_mutex_lock(&data->forks[id]);  // Lock the left fork
            pthread_mutex_lock(&data->forks[(id + 1) % data->nb_p]);  // Lock the right fork

            printf("Philosopher %d is eating.\n", id);
            philo->last_meal_time = get_current_time_in_ms();  // Update the last meal time
            usleep(1000 * data->eat_time);  // Use eat_time from data

            pthread_mutex_unlock(&data->forks[id]);  // Unlock the left fork
            pthread_mutex_unlock(&data->forks[(id + 1) % data->nb_p]);  // Unlock the right fork
        }

        // Thinking
        if (!data->someone_died)
            printf("Philosopher %d is thinking.\n", id);

        // Sleeping (split into smaller intervals for regular death checks)
        if (!data->someone_died)
        {
            printf("Philosopher %d is sleeping.\n", id);
            int sleep_interval = 10;  // Sleep in small intervals (100ms) to allow frequent checks
            for (int i = 0; i < data->sleep_time; i += sleep_interval)
            {
                usleep(1000 * sleep_interval);
                
                // Check if someone has died during sleep
                current_time = get_current_time_in_ms();
                if ((current_time - philo->last_meal_time) > data->die_time) {
                    pthread_mutex_lock(&data->death_mutex);
                    if (!data->someone_died) {
                        data->someone_died = 1;  // Set the death flag
                        printf("Philosopher %d has died.\n", id);
                    }
                    pthread_mutex_unlock(&data->death_mutex);
                    return NULL;  // End the simulation for this philosopher
                }
                pthread_mutex_lock(&data->death_mutex);
                if (data->someone_died) {
                    pthread_mutex_unlock(&data->death_mutex);
                    return NULL;
                }
                pthread_mutex_unlock(&data->death_mutex);
            }
        }

        // Check if the philosopher has died
        current_time = get_current_time_in_ms();
        if ((current_time - philo->last_meal_time) > data->die_time) {
            pthread_mutex_lock(&data->death_mutex);
            if (!data->someone_died) {
                data->someone_died = 1;  // Set the death flag
                printf("Philosopher %d has died.\n", id);
            }
            pthread_mutex_unlock(&data->death_mutex);
            break;  // End the simulation for this philosopher
        }
    }
    return NULL;
}


void setup_data(t_data *data, char **argv)
{
    data->nb_p = ft_atoi(argv[1]);
    data->die_time = ft_atoi(argv[2]);
    data->eat_time = ft_atoi(argv[3]);
    data->sleep_time = ft_atoi(argv[4]);
    data->someone_died = 0;  // Initialize the death flag
    data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_p);  // Allocate dynamic array for forks
    pthread_mutex_init(&data->death_mutex, NULL);  // Initialize the death mutex
}

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        printf("error\nhow to use : number_of_philosophers time_to_die time_to_eat time_to_sleep\n");
        return 1;
    }
    t_data data;
    setup_data(&data, argv);
    if (data.nb_p == 1)
    {
        usleep(1000 * data.die_time);
        printf("Philosopher 0 has died.\n");
        free(data.forks);
        return 0;
    }

    pthread_t philosophers[data.nb_p];
    t_philosopher philo_args[data.nb_p];

    // Initialize mutexes for the forks
    for (int i = 0; i < data.nb_p; i++) {
        pthread_mutex_init(&data.forks[i], NULL);
    }

    // Create threads for each philosopher
    for (int i = 0; i < data.nb_p; i++) {
        philo_args[i].id = i;
        philo_args[i].data = &data;
        philo_args[i].last_meal_time = get_current_time_in_ms();  // Initialize the last meal time
        pthread_create(&philosophers[i], NULL, philosopher, &philo_args[i]);
    }

    // Join threads (although in this example, the threads will exit when the first philosopher dies)
    for (int i = 0; i < data.nb_p; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy the mutexes when done
    for (int i = 0; i < data.nb_p; i++) {
        pthread_mutex_destroy(&data.forks[i]);
    }

    free(data.forks);  // Free the dynamically allocated array for forks

    return 0;
}


