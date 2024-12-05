/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberion <aberion@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 07:54:47 by aberion           #+#    #+#             */
/*   Updated: 2024/12/05 14:13:05 by aberion          ###   ########.fr       */
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
    long statring_time;
    int nb_p;
    int die_time;     // Time until a philosopher dies if they don't eat
    int eat_time;     // Time spent eating
    int sleep_time;   // Time spent sleeping
    int meal_nb;
    pthread_mutex_t *forks;    // Array of forks (mutexes)
    pthread_mutex_t *fork_status_mutex;
    pthread_mutex_t *eaten_mutex;
    pthread_mutex_t death_mutex;  // Mutex to protect the death flag
    pthread_mutex_t print_mutex;  // Mutex to protect printf calls
    pthread_mutex_t meal_mutex;
    pthread_mutex_t meal_c_mutex;
    int someone_died;  // Flag to indicate if a philosopher has died
    int meal_checker;
    int *fork_status;
    int *eaten_enough;
} t_data;

// Structure to hold the philosopher's ID and shared data
typedef struct s_philosopher
{
    int id;
    int meal_count;
    t_data *data;
    long last_meal_time;  // Timestamp of the last meal in milliseconds
} t_philosopher;

int	ft_atoi(const char *str);
long get_current_time_in_ms();
int skip_time(int time, t_data *data, t_philosopher *philo);
void man_down(t_data *data, t_philosopher *philo);
int     eat_routine(t_data *data, t_philosopher *philo);
void lock_forks(t_data *data, int id);
void unlock_forks(t_data *data, int id);
int is_someone_dead(t_data *data);
int sleep_routine(t_data *data, t_philosopher *philo);
int launch_parsing(int argc, char **argv);
int waiting_room(t_data *data, t_philosopher *philo);
int check_meal(t_data *data);
void set_meal(t_data *data, t_philosopher *philo);

#endif