/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philolets.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amonem <amonem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 22:36:06 by amonem            #+#    #+#             */
/*   Updated: 2023/10/16 19:42:01 by amonem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

uint64_t	gettime(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	main(int a, char **argv)
{
	t_all	*all;
	int		i;

	i = 0;
	if (a > 6 && a < 5)
	{
		write(2, "wrong argument\n", 15);
		return (0);
	}
	all = (t_all *)malloc(sizeof(t_all) * (ft_atoi(argv[1])));
	if (!(all))
		return (0);
	initilizetheinfo(all, argv, a);
	initilizingphilos(all, all->philossize);
	initilizethemutex(all);
	initilizethreads(all);
	detachall(all);
}
