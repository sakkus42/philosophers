#include "philo.h"

long int	ft_atoi(char *str)
{
	int			i;
	long int	dig;

	i = 0;
	dig = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		dig = (dig * 10) + (str[i] - 48);
		i++;
	}
	if (str[0] == '-' || str[i] == '+')
		return (dig * -1);
	return (dig);
}

void    fill_to_arg(int ac, char **arv, t_arg *arg)
{
	arg->nbr_of_philo = (int)ft_atoi(arv[1]);
	arg->time_die = (int)ft_atoi(arv[2]);
	arg->time_eat = (int)ft_atoi(arv[3]);
	arg->time_sleep = (int)ft_atoi(arv[4]);
	if (ac == 6)
		arg->total_eat = (int)ft_atoi(arv[5]);
	else
		arg->total_eat = 0;
}

int		is_dig(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

void	checker_arg(int ac, char *arv[])
{
	int	i;

	i = 1;
	while (i < ac)
	{
		if (!is_dig(arv[i++]))
		{
			printf("only numeric values are valid\n");
			exit(1);
		}
	}
}

void	mutex_init(int nbr_of_philo, t_philo *phil)
{
	int	i;

	i = 0;
	while (i < nbr_of_philo)
	{
		if (pthread_mutex_init(&phil[i++].mutex_Philo, NULL) != 0)
		{
			printf("Mutex Init Failed!\n");
			exit(1);
		}
	}
}

void	mutex_dest(int nbr_of_philo, t_philo *phil)
{
	int	i;

	i = 0;
	while (i < nbr_of_philo)
	{
		if (pthread_mutex_destroy(&phil[i++].mutex_Philo) != 0)
		{
			printf("Mutex Destory Failed!\n");
			exit(1);
		}
	}
}

void	fill_to_philo(t_arg *arg, t_philo *philo)
{
	struct timeval	ms;
	int				i;

	gettimeofday(&ms, NULL);
	i = 0;
	while (i < arg->nbr_of_philo)
	{
		philo[i].m_second = ms.tv_usec;
		philo[i].phil_ind = i + 1;
		philo[i++].arg = arg;
	}
}

 //philo sayısı kadar çatal olması lazım 
 //yemek yeme süresi bitmeden ölme zamanı gelirse direk ölmesi lazım.
 //philo yemek yerken iki çatal kullanır neden mi çünkü salaklar.
 //1 numaralı philo en son ki philonun yanında oturmaktadır => n - 1 n + 1 yuvarlak masa
 //Yer, düşünür veya uyurlar
 //Her eylemi yaparken diğer eylemleri yapmazlar.
 //Çatalları sağ ve sol eline alarak yerler
 //yemek->uyku->düşünmek
 //philo açlıktan öldüğünde simülasyon durur.
 //Her philonun yemek yemesi lazım asla aç kalmamalıdırlar.

 // PHİLO LOGS
 //timestamp_in_ms X has taken a fork
 //timestamp_in_ms X has is eating
 //timestamp_in_ms X has is sleeping
 //timestamp_in_ms X has is thinking
 //timestamp_in_ms X died => mili saniye içersinde öldü,
 
 // !!! data races !!!

 //philo sayısı kadar thread olması lazım; ok
 //eğer birden çok philo varsa sağ ve sol da çatal vardır. Tek philoda tek çatal var.
 //çatal durumlarını mutex ile korumaya almamız lazım.

// çatalları ver.
// çatalı olan yemeye başlasın diğeri dursun.
//
void	*routine(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;

	return (0);
}

void	thread_init(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->arg->nbr_of_philo)
	{
		if (pthread_create(&philo[i++].th_philo, NULL, &routine, NULL) != 0)
		{
			printf("Thread Creating Failed\n");
			exit(1);
		}
	}
	i = 0;
	while (i < philo->arg->nbr_of_philo)
	{
		if (pthread_join(philo[i++].th_philo, NULL) != 0)
		{
			printf("Thread joined Failed\n");
			exit(1);
		}
	}
}

int main(int ac, char *arv[])
{
	t_arg	arg;
	t_philo	philo[ft_atoi(arv[1])];

	if (ac < 5 || ac > 6)
	{
		printf("missing argument\n");
		return (0);
	}
	checker_arg(ac, arv);
	fill_to_arg(ac, arv, &arg);
	fill_to_philo(&arg, philo);
	mutex_init(arg.nbr_of_philo, philo);
	thread_init(philo);
	mutex_dest(arg.nbr_of_philo, philo);
}