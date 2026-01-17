.PHONY: all clean fclean

CC = cc

CFLAGS = -Wall -Werror -Wextra

CLIBS = -L./libft -lft -lreadline

LIBFT = libft/libft.a

BUILT_INS = ft_minishell_echo.c ft_minishell_pwd.c ft_minishell_env.c #ft_minishell_cd.c

LIBFT:
	make -C libft

# echo: ft_minishell_echo.c $(LIBFT)
# 	$(CC) $(CFLAGS) $^ $(CLIBS) -o echo

# pwd: ft_minishell_pwd.c $(LIBFT)
# 	$(CC) $(CFLAGS) $^ $(CLIBS) -o pwd

# env: ft_minishell_env.c $(LIBFT)
# 	$(CC) $(CFLAGS) $^ $(CLIBS) -o env

# cd: ft_minishell_cd.c $(LIBFT)
# 	$(CC) $(CFLAGS) $^ $(CLIBS) -o cd

test: $(BUILT_INS) ft_minishell_main.c
	$(CC) $(CFLAGS) $^ $(CLIBS) -o test

