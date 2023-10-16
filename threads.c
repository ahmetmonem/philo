/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amonem <amonem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 19:34:17 by amonem            #+#    #+#             */
/*   Updated: 2023/10/16 19:34:32 by amonem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
