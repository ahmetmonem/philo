/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_detach.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amonem <amonem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 19:33:12 by amonem            #+#    #+#             */
/*   Updated: 2023/10/16 19:41:34 by amonem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	initilizethemutex(t_all *all)
{
	t_all	*philo;
	int		i;

	i = 0;
	philo = all;
	pthread_mutex_init(&(all[i].deadstop), NULL);
	while (i < all->philossize)
	{
		pthread_mutex_init(&(all[i].deadmutex), NULL);
		pthread_mutex_init(&(all[i].fork), NULL);
		i++;
	}
}

void	initilizingphilos(t_all *all, int philosize)
{
	int	i;

	i = 0;
	while (i < philosize)
	{
		all[i].eating = 0;
		all[i].sleeping = 0;
		all[i].thinking = 0;
		all[i].deadstatus = 0;
		all[i].num = i;
		i++;
	}
}

void	initilizetheinfo(t_all *all, char **argv, int a)
{
	int	i;

	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		all->stime = gettime();
		all->eattime = ft_atoi(argv[3]) * 1000;
		all->sleeptime = ft_atoi(argv[4]) * 1000;
		all->philossize = ft_atoi(argv[1]);
		all->timetodie = ft_atoi(argv[2]);
		all->lasteattime = gettime();
		if (a == 6)
		{
			all->thereiseattime = 1;
			all->thenumbereaten = ft_atoi(argv[5]);
		}
		else if (a == 5)
		{
			all->thereiseattime = 0;
			all->thenumbereaten = 1;
		}
		all++;
		i++;
	}
}

void	initilizethreads(t_all *all)
{
	int	i;

	i = 0;
	while (i < all->philossize)
	{
		if (pthread_create(&(all[i].id), NULL, forallfunc, &all[i]))
			write(2, "Thread creation Error\n", 22);
		if (pthread_create(&(all[i].dead), NULL, checkdie, &all[i]))
			write(2, "Thread creation Error\n", 22);
		i++;
	}
	i = 0;
	while (i < all->philossize)
	{
		if (pthread_join(all[i].id, NULL))
			write(2, "Join Error\n", 11);
		if (pthread_join(all[i].dead, NULL))
			write(2, "Join Error\n", 11);
		i++;
	}
}

void	detachall(t_all *all)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&(all[i].deadstop));
	while (i < all->philossize)
	{
		pthread_mutex_destroy(&(all[i].fork));
		pthread_mutex_destroy(&(all[i].deadmutex));
		i++;
	}
	i = 0;
	while (i < all->philossize)
	{
		pthread_detach(all[i].id);
		pthread_detach(all[i].dead);
		i++;
	}
	free(all);
}
