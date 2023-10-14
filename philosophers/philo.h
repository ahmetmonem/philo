/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amonem <amonem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 15:46:49 by amonem            #+#    #+#             */
/*   Updated: 2023/10/14 16:34:51 by amonem           ###   ########.fr       */
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
	int				deadstatus;
    pthread_t		dead;
    uint64_t        lasteattime;
	pthread_mutex_t	fork;
	uint64_t		current_time;
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

// typedef struct	s_philo
// {
// 	pthread_t		thread;
// 	int				sleeping;
// 	int				eating;
// 	int				thinking;
// 	int				dead;
// 	pthread_mutex_t	fork;
// 	struct s_philo *left_philo;
// }					t_philo;

// typedef struct	s_philosmutex
// {
// 	pthread_mutex_t mutex;
// 	t_philo			philo;
// 	unsigned long	eating_time;
// 	unsigned long	sleep_time;
// 	unsigned long	thinking_time;
// 	unsigned long	dead_time;
// 	unsigned long	current_time;
// 	int				philosize;
// 	int				philo_num;
// }					t_philosmutex;