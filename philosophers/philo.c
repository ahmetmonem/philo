/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amonem <amonem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 23:21:28 by amonem            #+#    #+#             */
/*   Updated: 2023/10/02 20:32:10 by amonem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_philos
{
	pthread_mutex_t	my_mutex;
	pthread_t		thread;
	int				eattime;
	unsigned long			time;
}					t_philos;

int	ft_atoi(const char *str)
{
	int	i;
	int	n;
	int	j;

	i = 0;
	j = 0;
	n = 1;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			n = -1;
		i++;
	}
	if (str[i] != '\0' && (str[i] < '0' || str[i] > '9'))
		return (0);
	while (str[i] >= '0' && str[i] <= '9')
	{
		j = 10 * j + str[i] - 48;
		i++;
	}
	return (j * n);
}

void *to_beintothread(void *arg)
{
	t_philos *philo = (t_philos *) arg;
	struct timeval	tv;

	pthread_mutex_lock(&(philo->my_mutex));
	gettimeofday(&tv, NULL);
	philo->time = (tv.tv_sec * 1000000) + tv.tv_usec;
	printf("%lu\n" , philo->time);
	pthread_mutex_unlock(&(philo->my_mutex));
	usleep(2000000);
	pthread_mutex_lock(&(philo->my_mutex));
	gettimeofday(&tv, NULL);
	philo->time = (tv.tv_sec * 1000000) + tv.tv_usec;
	printf("%lu\n" , philo->time);
	pthread_mutex_unlock(&(philo->my_mutex));
	return(NULL);
}

int main(int a, char **arg)
{
	t_philos		*philo;
	int				i;
	int				j;
	struct timeval	tv;

	j = 0;
	i = 0;
	philo = (t_philos *)malloc(sizeof(t_philos) * ft_atoi(arg[1]));
	philo->eattime = ft_atoi(arg[2]);
	//gettimeofday(&tv, NULL);
		while (i < ft_atoi(arg[1]))
			pthread_mutex_init(&(philo[i++].my_mutex), NULL);
	i = 0;
	//philo.time = tv.tv_sec + tv.tv_usec;
	// philo[0].thread = (pthread_t *)malloc(sizeof(pthread_t) * ft_atoi(arg[1]));
	// philo[1].thread = (pthread_t *)malloc(sizeof(pthread_t) * ft_atoi(arg[1]));
	while (i < ft_atoi(arg[1]))
	{
		pthread_create(&(philo[i].thread), NULL, to_beintothread, &philo[i]);
		i++;
	}
	i = 0;
	while (i < a - 1)
	{

		pthread_join(philo[i++].thread, NULL);
	}
	return (0);
}
