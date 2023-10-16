/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkdie.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amonem <amonem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 19:36:02 by amonem            #+#    #+#             */
/*   Updated: 2023/10/16 19:36:12 by amonem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
