/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philowish.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amonem <amonem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 22:36:06 by amonem            #+#    #+#             */
/*   Updated: 2023/10/10 11:42:26 by amonem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

//number_of_philosophers ---- time_to_die ---- time_to_eat ---- time_to_sleep

typedef struct s_philo
{
	pthread_t	id;
	int	num;
	int	eating;
	int	sleeping;
	int	thinking;
	pthread_mutex_t	fork;
	pthread_mutex_t	doing;
}	t_philo;

typedef struct s_all
{
	t_philo			*philos;
	// pthread_mutex_t	mutexeat;
	uint64_t		current_time;
	int				eattime;
	int				sleeptime;
	int				timetodie;
	int				philossize;
	int				i;
}	t_all;

uint64_t	getting_time(void)
{
	struct timeval time;
	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	sleepingfunc(t_philo *philo, int sleepingtime, uint64_t current_time, int i)
{
	if (philo->sleeping == 1)
	{
		printf("%llu %d is sleeping \n", getting_time() - current_time, i + 1);
		usleep(sleepingtime);
		philo->sleeping = 0;
		philo->thinking = 1;
	}
}

void	iseating(pthread_mutex_t *fork1, pthread_mutex_t *fork2, uint64_t current_time, int eattime, int i, t_philo *philo)
{
		pthread_mutex_lock(fork1);
		//printf("%llu %d has a fork \n", getting_time() - current_time, i + 1);
		pthread_mutex_lock(fork2);
		//printf("%llu %d has a fork \n",  getting_time() - current_time, i + 1);
		philo->eating = 1;
		printf("%llu %d is eating \n", getting_time() - current_time, i + 1);
		usleep(eattime);
		pthread_mutex_unlock(fork1);
		philo->eating = 0;
		pthread_mutex_unlock(fork2);
		philo->sleeping = 1;
		sleepingfunc(philo, 100, current_time, i);//
}


void	eatingfunc(t_philo *philos, int philossize, uint64_t current_time, int eattime, int sleepingtime, int i)
{
	t_philo *temp;
	i = philos->num;
	while (i < philossize)
	{
		pthread_mutex_lock(&(philos[i].doing));
		if (philos[i].eating == 0 && philos[i].sleeping == 0)
		{
			if (i % 2)
			{
				if (i == philossize - 1)
				{
					temp = philos - i;
					iseating(&(philos[i].fork), &(temp[0].fork), current_time, eattime, i, &philos[i]);
				}
				else
					iseating(&(philos[i].fork), &(philos[i + 1].fork), current_time, eattime, i, &philos[i]);
			}
			else
			{	
				if (i == philossize - 1)
				{
					temp = philos - i;
					iseating(&(temp[0].fork), &(philos[i].fork), current_time, eattime, i, &philos[i]);
				}
				else
					iseating(&(philos[i + 1].fork), &(philos[i].fork), current_time, eattime, i, &philos[i]);
			}
		}
		pthread_mutex_unlock(&(philos[i].doing));
		i++;
	}
}

void	*forallfunc(void *arg)
{
	t_all *all = (t_all *) arg;
	t_philo *temp;
	
	temp = all->philos;
	while (1)
	{
		eatingfunc(all->philos, all->philossize, all->current_time, all->eattime, all->sleeptime, all->i);
	}
}

void	initilizethemutex(t_all *all)
{
	t_philo *philo = all->philos;
	int i;

	i = 0;
	while(i < all->philossize)
	{
		pthread_mutex_init(&(philo[i].fork), NULL);
		pthread_mutex_init(&(philo[i].doing), NULL);
		i++;
	}
}
void	initilizethethreads(t_all *all)
{
	t_philo *philo = all->philos;
	int i;

	i = 0;
	while(i < all->philossize)
	{
		philo = all->philos;
		pthread_create(&(philo->id), NULL, forallfunc, all);
		i++;
		all->philos = all->philos + 1;
	}
	all->philos = all->philos - i;
}

void	nulllast(t_philo **philos, int thesize)
{
	t_philo *last = *philos; 
	last = (*philos) + thesize;
	last = NULL;
}

void	initilizingphilos(t_philo *philo, int philosize)
{
	int	i;

	i = 0;
	while(i < philosize)
	{
		philo[i].eating = 0;
		philo[i].sleeping = 0;
		philo[i].thinking = 0;
		philo[i].num = i;
		i++;
	}
}

int main(int a, char **argv)
{
	t_all	all;
	int i;

	all.i = 0;
	i = 0;
	all.current_time = getting_time();
	all.eattime = ft_atoi(argv[3]) * 1000;
	all.sleeptime = ft_atoi(argv[4]) * 1000;
	all.philossize = ft_atoi(argv[1]);
	all.philos = (t_philo *)malloc(sizeof(t_philo) * (all.philossize) + 1);
	if (!(all.philos))
		return (0);
	initilizingphilos(all.philos, all.philossize);
	nulllast(&(all.philos), all.philossize);
	initilizethemutex(&all);
	initilizethethreads(&all);
	write(1, "11\n", 3);
	while(i < all.philossize)
	{
		pthread_join(all.philos[i].id, NULL);
		i++;
	}
}
