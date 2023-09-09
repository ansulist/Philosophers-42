#ifndef PHILO_H
#define PHILO_H
# include <pthread.h>
# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <sys/time.h>
# include <limits.h>

typedef struct s_philos t_philos;
typedef struct s_list t_list;

typedef enum e_status
{
    THINKING,
    SLEEPING,
    EATING,
    DEAD,
} t_status;

typedef struct s_list
{
    int to_die;
    int to_sleep;
    int to_eat;
    int n_philos;
    int eat_time;
    long long start_time;
    bool finish;
    t_philos    *philos;
    pthread_mutex_t sync;
    pthread_mutex_t *fork;
} t_list;

typedef struct s_philos
{
    pthread_t   thread;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    t_list  *av;
    int meal_taken;
    int id;
} t_philos;

int	ft_atoi(const char *str);
long long timestamp(void);

#endif