/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberion <aberion@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 07:54:47 by aberion           #+#    #+#             */
/*   Updated: 2024/12/19 15:18:21 by aberion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>


typedef struct s_data
{
	long			statring_time;
	int				nb_p;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				meal_nb;
	pthread_mutex_t	*forks;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	last_meal_mutex;
	pthread_mutex_t	meal_c_mutex;
	int				someone_died;
	int				*fork_status;
	int				*eaten_enough;
	struct s_philosopher	*philos;
}					t_data;

// Structure to hold the philosopher's ID and shared data
typedef struct s_philosopher
{
	int				id;
	int				meal_count;
	int				meal_nb;
	t_data			*data;
	long			last_meal_time;
}					t_philosopher;

int					ft_atoi(const char *str);
long				get_current_time_in_ms(void);
int					skip_time(int time, t_data *data, t_philosopher *philo);
void				man_down(t_data *data, t_philosopher *philo);
int					eat_routine(t_data *data, t_philosopher *philo);
int				lock_forks(t_data *data, t_philosopher *philo);
void				unlock_forks(t_data *data, int id);
int					is_someone_dead(t_data *data);
int					sleep_routine(t_data *data, t_philosopher *philo);
int					launch_parsing(int argc, char **argv);
int					waiting_room(t_data *data, t_philosopher *philo);
int					check_meal(t_data *data);
void				print_message(const char *s, t_data *data,
						t_philosopher *philo);
int					handle_meal_check(t_data *data, t_philosopher *philo);
int					handle_meal_state(t_data *data, t_philosopher *philo,
						int temp);
int					think_routine(t_data *data, t_philosopher *philo);
void				setup_data(t_data *data, char **argv);
void				cleanup_resources(t_data *data);
int					check_meal_checker(t_data *data);
int					check_meal_condition(t_data *data, t_philosopher *philo);
int					skip_time_eating(int time, t_data *data);
int					check_death_or_meal(t_data *data);
void error_cleanup(t_data *data, t_philosopher *philo_args, pthread_t *philosophers, int i);

#endif