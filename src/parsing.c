/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberion <aberion@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 13:18:53 by aberion           #+#    #+#             */
/*   Updated: 2024/10/30 13:57:51 by aberion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	ft_isdigit(int c)
{
	return (c >= 48 && c <= 57);
}

int launch_parsing(int argc, char **argv)
{
    int i;
    int j;
    if (argc != 5 && argc != 6)
    {
        printf("error\nhow to use : number_of_philosophers time_to_die time_to_eat time_to_sleep [opt : number_of_times_each_philosopher_must_eat]\n");
        return 1;
    }
    i = 1;
    j = 0;
    while(argv[i])
    {
        j = 0;
        while(argv[i][j])
        {
            if (!ft_isdigit(argv[i][j]))
            {
                printf("error\nnon numeric arg(s)");
                return 1;
            }
            j++;
        }
        i++;
    }
    return 0;
}
