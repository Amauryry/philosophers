/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberion <aberion@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 00:27:03 by aberion           #+#    #+#             */
/*   Updated: 2024/10/29 17:19:11 by aberion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"
#include <unistd.h>
#include <stdbool.h>

void *philosopher(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    int id = philo->id;
    t_data *data = philo->data;
    long current_time;

    while (1)
    {
        if(is_someone_dead(data))
            break;
        if (eat_routine(data, philo))
            break;
        if(is_someone_dead(data))
            break;
        current_time = get_current_time_in_ms();
        pthread_mutex_lock(&data->print_mutex);
        if (!is_someone_dead(data))
            printf("%ld %d is thinking\n", current_time, id);
        pthread_mutex_unlock(&data->print_mutex);

        if(sleep_routine(data, philo))
            break;
        current_time = get_current_time_in_ms();
        if ((current_time - philo->last_meal_time) > data->die_time)
        {
            man_down(philo);
            break; 
        }
    }
    return NULL;
}

void setup_data(t_data *data, char **argv)
{
    data->statring_time = get_current_time_in_ms();
    data->nb_p = ft_atoi(argv[1]);
    data->die_time = ft_atoi(argv[2]);
    data->eat_time = ft_atoi(argv[3]);
    data->sleep_time = ft_atoi(argv[4]);
    data->someone_died = 0;  // Initialize the death flag
    data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_p);  // Allocate dynamic array for forks
    pthread_mutex_init(&data->death_mutex, NULL);  // Initialize the death mutex
    pthread_mutex_init(&data->print_mutex, NULL);  // Initialize print mutex

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
    long starting_time = get_current_time_in_ms();
    long end_time;
    if (data.nb_p == 1)
    {
        usleep(1000 * data.die_time);
        printf("Philosopher 0 has died.\n");
        free(data.forks);
        end_time = get_current_time_in_ms();
        printf("checker time : %ld\n", end_time - starting_time);
        return 0;
    }

    pthread_t philosophers[data.nb_p];
    t_philosopher philo_args[data.nb_p];

    // Initialize mutexes for the forks
    for (int i = 0; i < data.nb_p; i++)
    {
        pthread_mutex_init(&data.forks[i], NULL);
    }

    // Create threads for each philosopher
    for (int i = 0; i < data.nb_p; i++)
    {
        philo_args[i].id = i;
        philo_args[i].data = &data;
        philo_args[i].last_meal_time = get_current_time_in_ms();  // Initialize the last meal time
        pthread_create(&philosophers[i], NULL, philosopher, &philo_args[i]);
        usleep(10000);
    }
    // printf("test2\n");
    // Join threads (although in this example, the threads will exit when the first philosopher dies)
    for (int i = 0; i < data.nb_p; i++)
    {
        pthread_join(philosophers[i], NULL);
    }
    pthread_mutex_destroy(&data.print_mutex);  // Destroy print mutex

    // Destroy the mutexes when done
    for (int i = 0; i < data.nb_p; i++)
    {
        pthread_mutex_destroy(&data.forks[i]);
    }
    free(data.forks);  // Free the dynamically allocated array for forks
    return 0;
}


