/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberion <aberion@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-10-18 00:27:03 by aberion           #+#    #+#             */
/*   Updated: 2024-10-18 00:27:03 by aberion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>   // for usleep
#include <stdlib.h>   // for malloc
#include <sys/time.h> // for gettimeofday

typedef struct s_data
{
    int nb_p;
    int die_time;     // Time until a philosopher dies if they don't eat
    int eat_time;     // Time spent eating
    int sleep_time;   // Time spent sleeping
} t_data;

pthread_mutex_t *forks;  // Array of forks (mutexes)
pthread_mutex_t death_mutex = PTHREAD_MUTEX_INITIALIZER;  // Mutex to protect the death flag
int someone_died = 0;  // Global flag to indicate if a philosopher has died

// Structure to hold the philosopher's ID and shared data
typedef struct s_philosopher
{
    int id;
    t_data *data;
    long last_meal_time;  // Timestamp of the last meal in milliseconds
} t_philosopher;

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
        pthread_mutex_lock(&death_mutex);
        if (someone_died) {
            pthread_mutex_unlock(&death_mutex);
            break;
        }
        pthread_mutex_unlock(&death_mutex);

        if (!someone_died)
        {
            pthread_mutex_lock(&forks[id]);  // Lock the left fork
            pthread_mutex_lock(&forks[(id + 1) % data->nb_p]);  // Lock the right fork

            printf("Philosopher %d is eating.\n", id);
            philo->last_meal_time = get_current_time_in_ms();  // Update the last meal time
            usleep(1000 * data->eat_time);  // Use eat_time from data

            pthread_mutex_unlock(&forks[id]);  // Unlock the left fork
            pthread_mutex_unlock(&forks[(id + 1) % data->nb_p]);  // Unlock the right fork
        }

        // Thinking
        if (!someone_died)
        {
            printf("Philosopher %d is thinking.\n", id);
            usleep(1000 * (rand() % 500));  // Random thinking time
        }

        // Sleeping (split into smaller intervals for regular death checks)
        if (!someone_died)
        {
            printf("Philosopher %d is sleeping.\n", id);
            int sleep_interval = 100;  // Sleep in small intervals (100ms) to allow frequent checks
            for (int i = 0; i < data->sleep_time; i += sleep_interval)
            {
                usleep(1000 * sleep_interval);
                
                // Check if someone has died during sleep
                current_time = get_current_time_in_ms();
                if ((current_time - philo->last_meal_time) > data->die_time) {
                    pthread_mutex_lock(&death_mutex);
                    if (!someone_died) {
                        someone_died = 1;  // Set the death flag
                        printf("Philosopher %d has died.\n", id);
                    }
                    pthread_mutex_unlock(&death_mutex);
                    return NULL;  // End the simulation for this philosopher
                }
                pthread_mutex_lock(&death_mutex);
                if (someone_died) {
                    pthread_mutex_unlock(&death_mutex);
                    return NULL;
                }
                pthread_mutex_unlock(&death_mutex);
            }
        }

        // Check if the philosopher has died
        current_time = get_current_time_in_ms();
        if ((current_time - philo->last_meal_time) > data->die_time) {
            pthread_mutex_lock(&death_mutex);
            if (!someone_died) {
                someone_died = 1;  // Set the death flag
                printf("Philosopher %d has died.\n", id);
            }
            pthread_mutex_unlock(&death_mutex);
            break;  // End the simulation for this philosopher
        }
    }
    return NULL;
}


void setup_data(t_data *data, char **argv)
{
    data->nb_p = atoi(argv[1]);
    data->die_time = atoi(argv[2]);
    data->eat_time = atoi(argv[3]);
    data->sleep_time = atoi(argv[4]);
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

    forks = malloc(sizeof(pthread_mutex_t) * data.nb_p);  // Allocate dynamic array for forks
    pthread_t philosophers[data.nb_p];
    t_philosopher philo_args[data.nb_p];

    // Initialize mutexes for the forks
    for (int i = 0; i < data.nb_p; i++) {
        pthread_mutex_init(&forks[i], NULL);
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
        pthread_mutex_destroy(&forks[i]);
    }

    free(forks);  // Free the dynamically allocated array for forks

    return 0;
}


