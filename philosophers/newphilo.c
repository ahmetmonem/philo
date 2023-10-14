/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newphilo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amonem <amonem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 15:17:27 by amonem            #+#    #+#             */
/*   Updated: 2023/10/07 22:38:16 by amonem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//number_of_philosophers ---- time_to_die ---- time_to_eat ---- time_to_sleep
unsigned long	ft_getthetime(void)
{
	struct timeval tv;
	unsigned long	time;
	
	gettimeofday(&tv, NULL);
	time =  tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (time);
}

void	sleeping(t_philo *philo, unsigned long sleep_time)
{
	philo->sleeping = 1;
	usleep(sleep_time);
	philo->sleeping = 0;
}

void	eating(t_philo *philo, unsigned long eating_time, unsigned long sleep_time, unsigned long current_time)
{
	philo->eating = 1;
	usleep(eating_time);
	philo->eating = 0;
}

void *test(void *arg)
{
	t_philosmutex	*philosmutex = (t_philosmutex *) arg;
	struct timeval tv;
	int		i = philosmutex->philo_num;
	int		befor;
	int		after;

while (1)
{
	pthread_mutex_lock(&(philosmutex->mutex));
	if (i == 0)
		befor = philosmutex->philosize;
	else
		befor = i - 1;
	if (i == philosmutex->philosize)
		after = 0;
	else
		after = i + 1;
	gettimeofday(&tv, NULL);
	if(philosmutex->philo[befor].eating == 0 && philosmutex->philo[after].eating == 0)
	{
		//eating(&(philosmutex->philo[i]), philosmutex->current_time, philosmutex->sleep_time);
		usleep(philosmutex->eating_time);
		printf("%lu %d is eating\n" , (tv.tv_sec * 1000 + tv.tv_usec / 1000) - philosmutex->current_time, i + 1);
	}
	pthread_mutex_unlock(&(philosmutex->mutex));
	// pthread_mutex_lock(&(philosmutex->mutex));
	// gettimeofday(&tv, NULL);
	// printf("%lu\n", tv.tv_sec * 1000000 + tv.tv_usec);
	// pthread_mutex_unlock(&(philosmutex->mutex));
}
	return (NULL);
}

void	initilizetosleep(t_philo *philo)
{
	philo->dead = 0;
	philo->eating = 0;
	philo->sleeping = 1;
	philo->thinking = 0;
}

void	initilizetozero(t_philo *philo)
{
	philo->dead = 0;
	philo->eating = 0;
	philo->sleeping = 0;
	philo->thinking = 0;
}

int main(int a, char **argv)
{
	t_philosmutex	philosmutex;
	int		i;

	philosmutex.dead_time = ft_atoi(argv[2]);
	philosmutex.eating_time = ft_atoi(argv[3]);
	philosmutex.sleep_time = ft_atoi(argv[4]);
	philosmutex.philosize = ft_atoi(argv[1]);
	philosmutex.current_time = ft_getthetime();
	i = 0;
	philosmutex.philo = (t_philo *)malloc(sizeof(t_philo) * ft_atoi(argv[1]) + 1);
	if (!philosmutex.philo)
		return (0);

	while (i < ft_atoi(argv[1]))
	{
		if (i % 2)
			initilizetozero(&(philosmutex.philo[i]));
		else
			initilizetosleep(&(philosmutex.philo[i]));
		i++;
	}
	pthread_mutex_init(&(philosmutex.mutex), NULL);
	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		philosmutex.philo_num = i;
		pthread_create(&(philosmutex.philo[i].thread), NULL, test, &philosmutex);
		i++;
	}
	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		pthread_join(philosmutex.philo[i].thread, NULL);
		i++;
	}
}