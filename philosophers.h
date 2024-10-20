/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberion <aberion@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-10-20 07:54:47 by aberion           #+#    #+#             */
/*   Updated: 2024-10-20 07:54:47 by aberion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>   // for usleep
# include <stdlib.h>   // for malloc
# include <sys/time.h> // for gettimeofday

typedef struct s_data
{
    int nb_p;
    int die_time;     // Time until a philosopher dies if they don't eat
    int eat_time;     // Time spent eating
    int sleep_time;   // Time spent sleeping
    pthread_mutex_t *forks;    // Array of forks (mutexes)
    pthread_mutex_t death_mutex;  // Mutex to protect the death flag
    int someone_died;  // Flag to indicate if a philosopher has died
} t_data;

// Structure to hold the philosopher's ID and shared data
typedef struct s_philosopher
{
    int id;
    t_data *data;
    long last_meal_time;  // Timestamp of the last meal in milliseconds
} t_philosopher;

#endif