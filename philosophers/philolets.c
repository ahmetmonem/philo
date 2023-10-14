/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philolets.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amonem <amonem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 22:36:06 by amonem            #+#    #+#             */
/*   Updated: 2023/10/14 17:12:59 by amonem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

//number_of_philosophers ---- time_to_die ---- time_to_eat ---- time_to_sleep

void	detachall(t_all *all)
{
	int i;

	i = 0;
	all -= all->num;
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

uint64_t	getting_time(void)
{
	struct timeval time;
	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	*checkdie(void *arg)
{
	int i;
	t_all *all = (t_all *) arg;

	i = all->num;
	while (1)
	{
		pthread_mutex_lock(&(all->deadmutex));
		if (getting_time() - all->lasteattime > all->timetodie)
		{
			pthread_mutex_lock(&(all - i)->deadstop);
			if (!(all->thenumbereaten))
			{
				pthread_mutex_unlock(&(all - i)->deadstop);
				return (NULL);
			}
			if ((all - i)->deadstatus == 1)
			{
				pthread_mutex_unlock(&(all - i)->deadstop);
				pthread_mutex_unlock(&(all->deadmutex));
				break;
			}
			printf("%llu %d died\n", getting_time() - all->current_time, i + 1);
			(all - i)->deadstatus = 1;
			pthread_mutex_unlock(&(all - i)->deadstop);
			pthread_mutex_unlock(&(all->deadmutex));
			return (NULL);
		}
		else
		{
			pthread_mutex_unlock(&(all->deadmutex));
		}
		pthread_mutex_lock(&(all - i)->deadstop);
		if ((all - i)->deadstatus == 1)
		{
			pthread_mutex_unlock(&(all - i)->deadstop);
			return (NULL);
		}
		pthread_mutex_unlock(&(all - i)->deadstop);	
	}
	return (NULL);
}

int	sleepingfunc(t_all *philo, int sleepingtime, uint64_t current_time, int i)
{
	pthread_mutex_lock(&((philo - i)->deadstop));
	if (philo->sleeping == 1 && (philo - i)->deadstatus == 0)
	{
		pthread_mutex_unlock(&((philo - i)->deadstop));
		printf("%llu %d is sleeping\n", getting_time() - current_time, i + 1);
		usleep(sleepingtime);
		pthread_mutex_lock(&((philo - i)->deadstop));
		if ((philo - i)->deadstatus == 0)
			printf("%llu %d is thinking\n", getting_time() - current_time, i + 1);
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
	else if ((philo - i)->deadstatus == 1)
	{
		pthread_mutex_unlock(&((philo - i)->deadstop));
		return (1);
	}
	else
		pthread_mutex_unlock(&((philo - i)->deadstop));
	return (1);
}

int	iseating(pthread_mutex_t *fork1, pthread_mutex_t *fork2, int i, t_all *philo)
{
		pthread_mutex_lock(fork1);
		pthread_mutex_lock(&((philo - i)->deadstop));
		if (!(philo->thenumbereaten) && philo->thereiseattime)
		{
			pthread_mutex_unlock(&((philo - i)->deadstop));
			return (1);
		}
		if ((philo - i)->deadstatus == 0)
			printf("%llu %d has taken a fork\n", getting_time() - philo->current_time, i + 1);
		else
		{
			pthread_mutex_unlock(fork1);
			pthread_mutex_unlock(&((philo - i)->deadstop));
			return (1);
		}
		pthread_mutex_unlock(&((philo - i)->deadstop));
		if (fork1 == fork2)
			return (1);
		pthread_mutex_lock(fork2);
		pthread_mutex_lock(&((philo - i)->deadstop));
		if ((philo - i)->deadstatus == 0)
			printf("%llu %d has taken a fork\n", getting_time() - philo->current_time, i + 1);
		else
		{
			pthread_mutex_unlock(fork1);
			pthread_mutex_unlock(&((philo - i)->deadstop));
			return (1);
		}
		pthread_mutex_unlock(&((philo - i)->deadstop));
		pthread_mutex_lock(&(philo->deadmutex));
		//printf("%llu %d has a fork\n",  getting_time() - philo->current_time, i + 1);
		philo->eating = 1;
		printf("%llu %d is eating\n", getting_time() - philo->current_time, i + 1);
		if (philo->thereiseattime && philo->thereiseattime)
			(philo->thenumbereaten)--;
		philo->lasteattime = getting_time();
		pthread_mutex_unlock(&(philo->deadmutex));
		usleep(philo->eattime);
		pthread_mutex_unlock(fork1);
		philo->eating = 0;
		pthread_mutex_unlock(fork2);
		philo->sleeping = 1;
		if (sleepingfunc(philo, philo->sleeptime, philo->current_time, i))
			return (1);
	return (0);
}


int	eatingfunc(t_all *all)
{
	t_all *temp;
	int i;

	i = all->num;
	pthread_mutex_lock(&((all - i)->deadstop));
	temp = all - i;
	if (all->eating == 0 && all->sleeping == 0 && (all - i)->deadstatus == 0)
	{
		pthread_mutex_unlock(&((all - i)->deadstop));
			if (i == all->philossize - 1)
			{
				if(iseating(&(all->fork), &(temp[0].fork), i, all))
					return (1);
			}
			else
			{
				if(iseating(&(all->fork), &(all[1].fork),  i, all))
					return (1);
			}
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
	t_all *all = (t_all *) arg;
	
	pthread_mutex_lock(&((all - all->num)->deadstop));
	while (all->deadstatus == 0)
	{
		pthread_mutex_unlock(&((all - all->num)->deadstop));
		if (eatingfunc(all))
			return NULL;
		pthread_mutex_lock(&((all - all->num)->deadstop));
	}
	pthread_mutex_unlock(&((all - all->num)->deadstop));
	return NULL;
}

void	initilizethemutex(t_all *all)
{
	t_all *philo = all;
	int i;

	i = 0;
	pthread_mutex_init(&(all[i].deadstop), NULL);
	while(i < all->philossize)
	{
		pthread_mutex_init(&(all[i].deadmutex), NULL);
		pthread_mutex_init(&(all[i].fork), NULL);
		i++;
	}
}

void	initilizethethreads(t_all *all)
{
	int i;

	i = 0;
	while(i < all->philossize)
	{
		pthread_create(&(all[i].id), NULL, forallfunc, &all[i]);
		pthread_create(&(all[i].dead), NULL, checkdie, &all[i]);
		i++;
	}
}

void	initilizingphilos(t_all *all, int philosize)
{
	int	i;

	i = 0;
	while(i < philosize)
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
	int i;

	i = 0;
	while(i < ft_atoi(argv[1]))
	{
		all->current_time = getting_time();
		all->eattime = ft_atoi(argv[3]) * 1000;
		all->sleeptime = ft_atoi(argv[4]) * 1000;
		all->philossize = ft_atoi(argv[1]);
		all->timetodie = ft_atoi(argv[2]);
		all->lasteattime = getting_time();
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

int main(int a, char **argv)
{
	t_all	*all;
	int i;

	i = 0;
	all = (t_all *)malloc(sizeof(t_all) * (ft_atoi(argv[1])));
	if (!(all))
		return (0);
	initilizetheinfo(all, argv, a);
	initilizingphilos(all, all->philossize);
	initilizethemutex(all);
	i = 0;
	while(i < all->philossize)
	{
		if(pthread_create(&(all[i].id), NULL, forallfunc, &all[i]))
			write(2, "Error\n", 6);
		if(pthread_create(&(all[i].dead), NULL, checkdie, &all[i]))
			write(2, "Error\n", 6);
		i++;
	}
	i = 0;
	while(i < all->philossize)
	{
		if(pthread_join(all[i].id, NULL))
			write(2, "Error\n", 6);
		if(pthread_join(all[i].dead, NULL))
		 	write(2, "Error\n", 6);
		i++;
	}
	detachall(all);
}
