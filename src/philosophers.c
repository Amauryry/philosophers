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

#include "../philosophers.h"
#include <unistd.h>

void *philosopher(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    int id = philo->id;
    t_data *data = philo->data;
    long current_time;

    while (1)
    {
        // Vérification si un philosophe est déjà mort
        pthread_mutex_lock(&data->death_mutex);
        if (data->someone_died)
        {
            pthread_mutex_unlock(&data->death_mutex);
            break;  // Quitte immédiatement si quelqu'un est mort
        }
        pthread_mutex_unlock(&data->death_mutex);

        // Philosophe prend les fourchettes et mange
        pthread_mutex_lock(&data->forks[id]);  // Verrouille la fourchette gauche
        pthread_mutex_lock(&data->forks[(id + 1) % data->nb_p]);  // Verrouille la fourchette droite

        current_time = get_current_time_in_ms();
        printf("%ld %d has taken a fork\n", current_time, id);
        printf("%ld %d is eating\n", current_time, id);
        philo->last_meal_time = current_time;  // Met à jour l'heure du dernier repas
        if (skip_time(data->eat_time, data, philo))  // Utiliser skip_time pour passer le temps de manger
            break;

        pthread_mutex_unlock(&data->forks[id]);  // Déverrouille la fourchette gauche
        pthread_mutex_unlock(&data->forks[(id + 1) % data->nb_p]);  // Déverrouille la fourchette droite

        pthread_mutex_lock(&data->death_mutex);
        if (data->someone_died)
        {
            pthread_mutex_unlock(&data->death_mutex);
            break;
        }
        pthread_mutex_unlock(&data->death_mutex);

        // Philosophe pense
        current_time = get_current_time_in_ms();
        printf("%ld %d is thinking\n", current_time, id);

        // Philosophe dort en vérifiant fréquemment la condition de mort
        current_time = get_current_time_in_ms();
        printf("%ld %d is sleeping\n", current_time, id);
        if (skip_time(data->sleep_time, data, philo))  // Utiliser skip_time pour passer le temps de sommeil
            break;

        // Dernière vérification de la mort après avoir dormi
        current_time = get_current_time_in_ms();
        if ((current_time - philo->last_meal_time) > data->die_time) {
            man_down(philo);  // Appelle man_down pour imprimer la mort seulement pour ce philosophe
            break;  // Quitte la boucle pour ce philosophe
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
    for (int i = 0; i < data.nb_p; i++) {
        pthread_mutex_init(&data.forks[i], NULL);
    }

    // Create threads for each philosopher
    for (int i = 0; i < data.nb_p; i++) {
        if(data.someone_died)
            break;
        philo_args[i].id = i;
        philo_args[i].data = &data;
        philo_args[i].last_meal_time = get_current_time_in_ms();  // Initialize the last meal time
        pthread_create(&philosophers[i], NULL, philosopher, &philo_args[i]);
    }
    printf("test2\n");
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


