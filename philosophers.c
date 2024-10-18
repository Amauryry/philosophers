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

#define N 5  // Number of philosophers and forks

typedef struct s_data
{
    int nb_p;
    int die_time;
    int eat_time;
    int sleep_time;
}   t_data;

pthread_mutex_t forks[N];  // One fork between each pair of philosophers

// Function that simulates the philosopher's actions
void *philosopher(void *arg) {
    int id = *(int *)arg;  // Philosopher's ID

    while (1) {
        // Thinking
        printf("Philosopher %d is thinking.\n", id);
        usleep(1000 * (rand() % 500));  // Random thinking time

        // Picking up forks (mutexes)
        pthread_mutex_lock(&forks[id]);  // Lock the left fork
        pthread_mutex_lock(&forks[(id + 1) % N]);  // Lock the right fork

        // Eating
        printf("Philosopher %d is eating.\n", id);
        usleep(1000 * (rand() % 500));  // Random eating time

        // Putting down forks
        pthread_mutex_unlock(&forks[id]);  // Unlock the left fork
        pthread_mutex_unlock(&forks[(id + 1) % N]);  // Unlock the right fork

        // Sleeping
        printf("Philosopher %d is sleeping.\n", id);
        usleep(1000 * (rand() % 500));  // Random sleeping time
    }
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
    pthread_t philosophers[data.nb_p];
    int ids[data.nb_p];

    // Initialize mutexes for the forks
    for (int i = 0; i < data.nb_p; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    // Create threads for each philosopher
    for (int i = 0; i < data.nb_p; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    // Join threads (although in this example, the threads run indefinitely)
    for (int i = 0; i < data.nb_p; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy the mutexes when done (though unreachable here)
    for (int i = 0; i < data.nb_p; i++) {
        pthread_mutex_destroy(&forks[i]);
    }

    return 0;
}
