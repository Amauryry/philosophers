/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberion <aberion@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 13:18:53 by aberion           #+#    #+#             */
/*   Updated: 2024/12/09 14:22:11 by aberion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	ft_isdigit(int c)
{
	return (c >= 48 && c <= 57);
}

int	are_param_g(int argc, char **argv)
{
	if (ft_atoi(argv[1]) < 0 || ft_atoi(argv[1]) > 200)
		return (1);
	if (ft_atoi(argv[2]) < 0)
		return (1);
	if (ft_atoi(argv[3]) < 0)
		return (1);
	if (argc == 6)
	{
		if (ft_atoi(argv[4]) < 0)
			return (1);
	}
	return (0);
}

int	launch_parsing(int argc, char **argv)
{
	int	i;
	int	j;

	if ((argc != 5 && argc != 6) || are_param_g(argc, argv))
		return (1);
	i = 1;
	j = 0;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (!ft_isdigit(argv[i][j]))
				return (1);
			j++;
			if (j > 10)
				return (1);
		}
		i++;
	}
	return (0);
}
