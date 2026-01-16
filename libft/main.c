#include "libft.h"
#include <stdio.h>
#include <limits.h>
#include <fcntl.h>

// typedef struct s_listn
// {
// 	int				content;
// 	struct s_listn	*next;
// }	t_listn;

int	main()
{
	ft_printf("ft_atoi_base(hex): %d\n", ft_atoi_base("2a", "0123456789abcdef"));
	ft_printf("ft_atoi_base(hex): %d\n", ft_atoi_base("-2a", "0123456789abcdef"));
	ft_printf("ft_atoi_base(hex): %d\n", ft_atoi_base("7fffffff", "0123456789abcdef"));
	ft_printf("ft_atoi_base(hex): %d\n", ft_atoi_base("-80000000", "0123456789abcdef"));
	ft_printf("ft_atoi_base(dec): %d\n", ft_atoi_base("42", "0123456789"));
	ft_printf("ft_atoi_base(dec): %d\n", ft_atoi_base("-42", "0123456789"));
	ft_printf("ft_atoi_base(dec): %d\n", ft_atoi_base("2147483647", "0123456789"));
	ft_printf("ft_atoi_base(dec): %d\n", ft_atoi_base("-2147483648", "0123456789"));
	ft_printf("ft_atoi_base(bin): %d\n", ft_atoi_base("101010", "01"));
	ft_printf("ft_atoi_base(bin): %d\n", ft_atoi_base("-101010", "01"));
	ft_printf("ft_atoi_base(bin): %d\n", ft_atoi_base("1111111111111111111111111111111", "01"));
	ft_printf("ft_atoi_base(bin): %d\n", ft_atoi_base("-10000000000000000000000000000000", "01"));
	
	char	*str_num;
	str_num = ft_itoa(42);
	ft_printf("ft_itoa: %s\n", str_num);
	free(str_num);
	str_num = ft_itoa(-42);
	ft_printf("ft_itoa: %s\n", str_num);
	free(str_num);
	str_num = ft_itoa(0);
	ft_printf("ft_itoa: %s\n", str_num);
	free(str_num);
	str_num = ft_itoa(-4);
	ft_printf("ft_itoa: %s\n", str_num);
	free(str_num);
	str_num = ft_itoa(2);
	ft_printf("ft_itoa: %s\n", str_num);
	free(str_num);
	str_num = ft_itoa(INT_MIN);
	ft_printf("ft_itoa: %s\n", str_num);
	free(str_num);
	str_num = ft_itoa(INT_MAX);
	ft_printf("ft_itoa: %s\n", str_num);
	free(str_num);

	char **spl = ft_split("hola que tal", ' ');
	for (int i = 0; spl && spl[i]; i++)
		printf("ft_split: %s\n", spl[i]);
	free_split(spl);
	spl = ft_split(" hola que tal ", ' ');
	for (int i = 0; spl && spl[i]; i++)
		printf("ft_split: %s\n", spl[i]);
	free_split(spl);
	spl = ft_split("   hola   que   tal   ", ' ');
	for (int i = 0; spl && spl[i]; i++)
		printf("ft_split: %s\n", spl[i]);
	free_split(spl);
	spl = ft_split("      ", ' ');
	for (int i = 0; spl && spl[i]; i++)
		printf("ft_split: %s\n", spl[i]);
	free_split(spl);
	spl = ft_split("", ' ');
	for (int i = 0; spl && spl[i]; i++)
		printf("ft_split: %s\n", spl[i]);
	free_split(spl);
	spl = ft_split(NULL, ' ');
	for (int i = 0; spl && spl[i]; i++)
		printf("ft_split: %s\n", spl[i]);
	free_split(spl);

	int fd = open("libft.h", O_RDONLY);
	ft_printf("fd: %d\n", fd);
	for (int i = 0; i < 11; i++)
	{
		char *gnl = get_next_line(fd);
		ft_printf("gnl: %s", gnl);
		free(gnl);
	}

	fd = open("void", O_RDONLY);
	ft_printf("fd: %d\n", fd);
	for (int i = 0; i < 11; i++)
	{
		char *gnl = get_next_line(fd);
		ft_printf("gnl: %s", gnl);
		free(gnl);
	}
	return (0);
}

// void main_list()
// {
	// t_listn	*lst;
	// t_listn	*new;
	// t_listn	*aux;

	// lst = malloc(sizeof(t_list));
	// lst->content = 5;

	// aux = lst;
	// for (int i = 0; i < 50; i++)
	// {
	// 	new = malloc(sizeof(t_list));
	// 	new->content = (i*73+11) % 101 ;
	// 	aux->next = new;
	// 	aux = new;
	// }

	// new->next = NULL;

	// aux = lst;
	// while (aux)
	// {
	// 	printf("%d\n", aux->content);
	// 	aux = aux->next;
	// }
// }