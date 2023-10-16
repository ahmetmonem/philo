/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philolets.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amonem <amonem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 22:36:06 by amonem            #+#    #+#             */
/*   Updated: 2023/10/16 18:18:15 by amonem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

uint64_t	gettime(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	checkdiehelper(t_all *all, int i)
{
	pthread_mutex_lock(&(all - i)->deadstop);
	if (!(all->thenumbereaten))
	{
		pthread_mutex_unlock(&(all - i)->deadstop);
		return (1);
	}
	if ((all - i)->deadstatus == 1)
	{
		pthread_mutex_unlock(&(all - i)->deadstop);
		pthread_mutex_unlock(&(all->deadmutex));
		return (-1);
	}
	printf("%llu %d died\n", gettime() - all->stime, i + 1);
	(all - i)->deadstatus = 1;
	pthread_mutex_unlock(&(all - i)->deadstop);
	pthread_mutex_unlock(&(all->deadmutex));
	return (1);
}

void	*checkdie(void *arg)
{
	int		ret;
	t_all	*all;

	all = (t_all *) arg;
	while (1)
	{
		pthread_mutex_lock(&(all->deadmutex));
		if (gettime() - all->lasteattime > all->timetodie)
		{
			ret = checkdiehelper(all, all->num);
			if (ret == 1)
				return (NULL);
			else if (ret == -1)
				break ;
		}
		else
			pthread_mutex_unlock(&(all->deadmutex));
		pthread_mutex_lock(&(all - all->num)->deadstop);
		if ((all - all->num)->deadstatus == 1)
		{
			pthread_mutex_unlock(&(all - all->num)->deadstop);
			return (NULL);
		}
		pthread_mutex_unlock(&(all - all->num)->deadstop);
	}
	return (NULL);
}

int	sleepingfunc(t_all *philo, int sleepingtime, uint64_t stime, int i)
{
	pthread_mutex_lock(&((philo - i)->deadstop));
	if (philo->sleeping == 1 && (philo - i)->deadstatus == 0)
	{
		pthread_mutex_unlock(&((philo - i)->deadstop));
		printf("%llu %d is sleeping\n", gettime() - stime, i + 1);
		usleep(sleepingtime);
		pthread_mutex_lock(&((philo - i)->deadstop));
		if ((philo - i)->deadstatus == 0)
			printf("%llu %d is thinking\n", gettime() - stime, i + 1);
		else
		{
			pthread_mutex_unlock(&((philo - i)->deadstop));
			return (1);
		}
		pthread_mutex_unlock(&((philo - i)->deadstop));
		philo->sleeping = 0;
		philo->thinking = 1;
		return (0);
	}
	else
		pthread_mutex_unlock(&((philo - i)->deadstop));
	return (1);
}

int	take_fork1(t_all *all1, t_all *all2, int i, t_all *philo)
{
	pthread_mutex_lock(&(all1->fork));
	if (i == philo->philossize - 1 || i % 2 == 0)
		usleep(150);
	pthread_mutex_lock(&((philo - i)->deadstop));
	if (!(philo->thenumbereaten) && philo->thereiseattime)
	{
		pthread_mutex_unlock(&((philo - i)->deadstop));
		pthread_mutex_unlock(&(all1->fork));
		return (1);
	}
	if ((philo - i)->deadstatus == 0)
		printf("%llu %d has taken a fork\n", gettime() - philo->stime, i + 1);
	else
	{
		pthread_mutex_unlock(&(all1->fork));
		pthread_mutex_unlock(&((philo - i)->deadstop));
		return (1);
	}
	pthread_mutex_unlock(&((philo - i)->deadstop));
	if (&(all1->fork) == &(all2->fork))
	{
		pthread_mutex_unlock(&(all1->fork));
		return (1);
	}
	return (0);
}

int	take_fork2(t_all *all1, t_all *all2, int i, t_all *philo)
{
	pthread_mutex_lock(&(all2->fork));
	pthread_mutex_lock(&((philo - i)->deadstop));
	if ((philo - i)->deadstatus == 0)
		printf("%llu %d has taken a fork\n", gettime() - philo->stime, i + 1);
	else
	{
		pthread_mutex_unlock(&(all1->fork));
		pthread_mutex_unlock(&((philo - i)->deadstop));
		return (1);
	}
	pthread_mutex_unlock(&((philo - i)->deadstop));
	pthread_mutex_lock(&(philo->deadmutex));
	philo->eating = 1;
	printf("%llu %d is eating\n", gettime() - philo->stime, i + 1);
	if (philo->thereiseattime && philo->thereiseattime)
		(philo->thenumbereaten)--;
	philo->lasteattime = gettime();
	pthread_mutex_unlock(&(philo->deadmutex));
	usleep(philo->eattime);
	pthread_mutex_unlock(&(all1->fork));
	if (i == philo->philossize - 1 || i % 2 == 0)
		usleep(150);
	philo->eating = 0;
	pthread_mutex_unlock(&(all2->fork));
	philo->sleeping = 1;
	return (0);
}

int	iseating(t_all *all1, t_all *all2, int i, t_all *philo)
{
	int	ret1;
	int	ret2;

	ret1 = take_fork1(all1, all2, i, philo);
	if (ret1 == 1)
		return (1);
	ret2 = take_fork2(all1, all2, i, philo);
	if (ret2 == 1)
		return (1);
	if (sleepingfunc(philo, philo->sleeptime, philo->stime, i))
		return (1);
	return (0);
}

int	wait_and_startlast(t_all *all, int i, t_all *temp)
{
	if (i % 2)
		usleep(200);
	if (i != 1)
	{
		if (iseating(temp, all, i, all))
			return (1);
	}
	else
	{
		if (iseating(all, temp, i, all))
			return (1);
	}
	return (0);
}

int	wait_and_start(t_all *all, int i)
{
	if (i % 2 == 0)
	{
		if (iseating(&(all[1]), all, i, all))
			return (1);
	}
	else
	{
		usleep(200);
		if (iseating(all, &(all[1]), i, all))
			return (1);
	}
	return (0);
}

int	allfunc(t_all *all)
{
	t_all	*temp;
	int		i;
	int		ret;

	i = all->num;
	pthread_mutex_lock(&((all - i)->deadstop));
	temp = all - i;
	if (all->eating == 0 && all->sleeping == 0 && (all - i)->deadstatus == 0)
	{
		pthread_mutex_unlock(&((all - i)->deadstop));
		if (i == all->philossize - 1)
			ret = wait_and_startlast(all, i, temp);
		else
			ret = wait_and_start(all, i);
		if (ret == 1)
			return (1);
	}
	else if ((all - i)->deadstatus == 1)
	{
		pthread_mutex_unlock(&((all - i)->deadstop));
		return (1);
	}
	else
		pthread_mutex_unlock(&((all - i)->deadstop));
	return (0);
}

void	*forallfunc(void *arg)
{
	t_all	*all;

	all = (t_all *) arg;
	pthread_mutex_lock(&((all - all->num)->deadstop));
	while (all->deadstatus == 0)
	{
		pthread_mutex_unlock(&((all - all->num)->deadstop));
		if (allfunc(all))
			return (NULL);
		pthread_mutex_lock(&((all - all->num)->deadstop));
	}
	pthread_mutex_unlock(&((all - all->num)->deadstop));
	return (NULL);
}

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
			write(2, "Error\n", 6);
		if (pthread_create(&(all[i].dead), NULL, checkdie, &all[i]))
			write(2, "Error\n", 6);
		i++;
	}
	i = 0;
	while (i < all->philossize)
	{
		if (pthread_join(all[i].id, NULL))
			write(2, "Error\n", 6);
		if (pthread_join(all[i].dead, NULL))
			write(2, "Error\n", 6);
		i++;
	}
}

int	main(int a, char **argv)
{
	t_all	*all;
	int		i;

	i = 0;
	all = (t_all *)malloc(sizeof(t_all) * (ft_atoi(argv[1])));
	if (!(all))
		return (0);
	initilizetheinfo(all, argv, a);
	initilizingphilos(all, all->philossize);
	initilizethemutex(all);
	initilizethreads(all);
	detachall(all);
}
