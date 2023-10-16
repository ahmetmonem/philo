/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_do.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amonem <amonem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 19:35:13 by amonem            #+#    #+#             */
/*   Updated: 2023/10/16 19:35:24 by amonem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
