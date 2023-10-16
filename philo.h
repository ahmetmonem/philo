/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amonem <amonem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 15:46:49 by amonem            #+#    #+#             */
/*   Updated: 2023/10/16 19:45:47 by amonem           ###   ########.fr       */
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
	pthread_t		id;
	int				num;
	int				eating;
	int				sleeping;
	int				thinking;
	int				forkstatus;
	int				deadstatus;
	pthread_t		dead;
	uint64_t		lasteattime;
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

int			ft_atoi(const char *str);
uint64_t	getting_time(void);
void		detachall(t_all *all);
int			checkdiehelper(t_all *all, int i);
void		*checkdie(void *arg);
int			sleepingfunc(t_all *philo, int sleepingtime, uint64_t stime, int i);
int			take_fork1(t_all *all1, t_all *all2, int i, t_all *philo);
int			take_fork2(t_all *all1, t_all *all2, int i, t_all *philo);
int			iseating(t_all *all1, t_all *all2, int i, t_all *philo);
int			wait_and_startlast(t_all *all, int i, t_all *temp);
int			wait_and_start(t_all *all, int i);
int			allfunc(t_all *all);
void		*forallfunc(void *arg);
void		initilizethemutex(t_all *all);
void		initilizingphilos(t_all *all, int philosize);
void		initilizetheinfo(t_all *all, char **argv, int a);
void		initilizethreads(t_all *all);

#endif

// -fsanitize=thread
