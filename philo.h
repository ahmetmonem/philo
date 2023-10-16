/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amonem <amonem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 15:46:49 by amonem            #+#    #+#             */
/*   Updated: 2023/10/16 18:12:03 by amonem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>


typedef struct s_all
{
	pthread_t	id;
	int	num;
	int	eating;
	int	sleeping;
	int	thinking;
	int	forkstatus;
	int				deadstatus;
    pthread_t		dead;
    uint64_t        lasteattime;
	pthread_mutex_t	fork;
	uint64_t		stime;
	int				eattime;
	int				sleeptime;
	uint64_t		timetodie;
	int				philossize;
	pthread_mutex_t	deadmutex;
	pthread_mutex_t	deadstop;
	int				thenumbereaten;
	int				thereiseattime;
}	t_all;

int	ft_atoi(const char *str);
uint64_t	getting_time(void);

#endif


// -fsanitize=thread
