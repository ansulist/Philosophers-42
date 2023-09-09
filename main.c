#include "philo.h"

int error_message(void)
{
    printf("Number is wrong\n");
    printf("format is = \n");
    return (0);
}

int is_digit(char c)
{
    if (c >= '0' && c <= '9')
        return (0);
    return (-1);
}

// checking if the arguments is only digits
int check_arguments(char **av)
{
    int i;
    int j;

    i = 1;
    while (av[i] != NULL)
    {
        j = 0;
        while (av[i][j] != '\0')
        {
            if (is_digit(av[i][j]) == -1)
                return (-1);
            j++;
        }
        i++;
    }
    return (0);
}

// checkinh the number of how many arguments
int arguments(int ac, char **av)
{
    if (ac != 5 && ac != 6)
    {
        error_message();
        return (-1);
    }
    if (check_arguments(av) == -1)
    {
        error_message();
        return (-1);
    }
    return (0);
}

// chceking the max of each aguments for the algorithm
int check_max(t_list *list, char **av)
{
    if (list->n_philos < 0 || list->to_die < 0 || list->to_eat < 0 || list->to_sleep < 0 || list->n_philos > 200)
        return (-1);
    if (av[5] != NULL)
    {
        list->eat_time = ft_atoi(av[5]);
        if (list->eat_time < 0)
            return (-1);
    }
    else
        list->eat_time = -1;
    return (0);
}

// to initialize mutex for each philo
// when they do the activity
int init_mutex(pthread_mutex_t *fork, int size)
{
    int i;

    i = 0;
    while (i < size)
    {
        if (pthread_mutex_init(&fork[i], NULL) == -1)
            return (-1);
        i++;
    }
    return (0);
}

int init_philos(t_list *list)
{
    int i;

    i = 0;
    while (i < list->n_philos)
    {
        list->philos[i].av = list;
        list->philos[i].left_fork = &list->fork[i];
        list->philos[i].right_fork = &list->fork[(list->n_philos - 1 + i) % list->n_philos];
        list->philos[i].id = i;
        list->philos[i].meal_taken = 0;
        i++;
    }
    return (0);
}

// ini struct for filling each value of the variable
// in the structure
int init_struct(char **av, t_list *list)
{
    list->n_philos = ft_atoi(av[1]);
    list->to_die = ft_atoi(av[2]);
    list->to_eat = ft_atoi(av[3]);
    list->to_sleep = ft_atoi(av[4]);
    list->start_time = timestamp();
    if (check_max(list, av) == -1)
    {
        printf("Number is invalid\n");
        return (-1);
        // to do freeing the struct
    }
    list->philos = malloc(sizeof(t_philos) * list->n_philos);
    list->fork = malloc(sizeof(pthread_mutex_t) * list->n_philos);
    init_mutex(list->fork, list->n_philos);
    pthread_mutex_init(&list->sync, NULL);
    init_philos(list);
    return (0);
}

// void print_struct(t_list *list)
// {
//     printf("n_philo = %d\n", list->n_philos);
//     printf("to eat = %d\n", list->to_eat);
//     printf("to sleep = %d\n", list->to_sleep);
//     printf("to die = %d\n", list->to_die);
//     printf("eat times = %d\n", list->eat_time);
// }

// give timestamp in ms
long long timestamp(void)
{
    struct timeval time;

    gettimeofday(&time, NULL);
    return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

bool check_meal_taken(t_list *list)
{
    int i;

    if (list->eat_time == -1)
    {
        return (false);
    }

    i = 0;
    while (list->n_philos > i)
    {
        if (list->philos[i].meal_taken < list->eat_time)
            return (false);
        i++;
    }
    return (true);
}

void report_action(char *action, t_philos *philosopher, t_list *list, char *color)
{
    if (list->finish == true)
        return;

    printf("%lld : %sphilo %d : %s\n\e[0m", (timestamp() - list->start_time), color, philosopher->id, action);
    fflush(0);
}

void philo_eats(t_list *list, t_philos *philosopher)
{
    pthread_mutex_unlock(philosopher->right_fork);
    pthread_mutex_unlock(philosopher->left_fork);
    pthread_mutex_lock(&list->sync);
    philosopher->meal_taken++;
    if (check_meal_taken(list) == true)
        list->finish = true;
    pthread_mutex_unlock(&list->sync);
    //  printf("%lld ", time);
    //  report_action("\x1b[36mis finished eating", philosopher, list);
}

void *algorithm(void *value)
{
    t_philos *philosophers;
    t_list *list;
    int status;
    long long time;
    long long last_meal;

    status = THINKING;
    philosophers = (t_philos *)value;
    list = philosophers->av;
    last_meal = timestamp();
    while (status != DEAD)
    {
        if (list->finish)
        {
            pthread_mutex_unlock(philosophers->right_fork);
            pthread_mutex_unlock(philosophers->left_fork);
            return (NULL);
        }
        if ((timestamp() - last_meal) >= list->to_die)
        {
            // printf("before  helo from the other side\n");
            pthread_mutex_lock(&list->sync);
            // printf("apres helo from the other side\n");
            status = DEAD;
            report_action("is dead", philosophers, list, "\x1b[31m");
            list->finish = true;
            pthread_mutex_unlock(philosophers->right_fork);
            pthread_mutex_unlock(philosophers->left_fork);
            pthread_mutex_unlock(&list->sync);
            // printf("helo from the other side\n");
            return (NULL);
        }
        if (status == THINKING)
        {
            if (philosophers->id % 2 == 0)
            {
                pthread_mutex_lock(philosophers->left_fork);
                report_action("has taken left fork", philosophers, list, "\x1b[33m");
                pthread_mutex_lock(philosophers->right_fork);
                report_action("has taken right fork\e[0m", philosophers, list, "\x1b[35m");
            }
            else
            {
                pthread_mutex_lock(philosophers->right_fork);
                report_action("has taken right fork", philosophers, list, "\x1b[35m");
                pthread_mutex_lock(philosophers->left_fork);
                report_action("has taken left fork", philosophers, list, "\x1b[33m");
            }
            status = EATING;
            time = timestamp();
            if (list->finish)
            {
                pthread_mutex_unlock(philosophers->right_fork);
                pthread_mutex_unlock(philosophers->left_fork);
                return (NULL);
            }
            report_action("is start eating", philosophers, list, "\x1b[37m");
        }
        else if (status == EATING)
        {
            if (timestamp() - time >= list->to_eat)
            {
                status = SLEEPING;
                time = timestamp();
                last_meal = time;
                report_action("is finished eating", philosophers, list, "\x1b[36m");
                philo_eats(list, philosophers);
            }
        }
        else if (status == SLEEPING)
        {
            if (timestamp() - time >= list->to_sleep)
            {
                status = THINKING;
                time = timestamp();
                report_action("is thinking", philosophers, list, "\x1b[32m");
            }
        }
        usleep(100);
    }
    return (NULL);
}

void    one_algorithm(void *value)
{
    t_philos *philosophers;
    t_list *list;

    philosophers = (t_philos *)value;
    list = philosophers->av;
    pthread_mutex_lock(philosophers->left_fork);
    report_action("has taken left fork", philosophers, list, "\x1b[33m");
    while ((timestamp() - list->start_time) < list->to_die) {
        usleep(100);
    }
    pthread_mutex_unlock(philosophers->left_fork);
    report_action("is dead", philosophers, list, "\x1b[31m");
    list->finish = true;
    return ;
}

int start_algorithm(t_list *list)
{
    int i;

    if (list->n_philos == 1)
        one_algorithm(list->philos);
    i = 0;
    while (i < list->n_philos)
    {
        if (pthread_create(&list->philos[i].thread, NULL, &algorithm, (void *)&list->philos[i]) == -1)
            return (-1);
        i++;
    }
    return (0);
}

void ft_clean_all(t_list *list)
{
    int i;

    i = 0;
    free(list->philos);
    while (list->n_philos > i)
    {
        pthread_mutex_destroy(&list->fork[i]);
        i++;
    }
    pthread_mutex_destroy(&list->sync);
    free(list->fork);
}

int main(int ac, char **av)
{
    t_list *list;
    int i;

    i = 0;
    if (arguments(ac, av) == -1)
        return (-1);
    list = malloc(sizeof(t_list) * 1);
    if (init_struct(av, list) == -1)
        return (-1);
    start_algorithm(list);
    while (i < list->n_philos)
    {
        pthread_join(list->philos[i].thread, NULL);
        i++;
    }
    ft_clean_all(list);
    free(list);
    return (0);
}