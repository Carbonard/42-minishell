.PHONY: all clean fclean

CC = cc

CFLAGS = -Wall -Werror -Wextra -g3 #-fsanitize=address

CLIBS = -L./libft -L./dynamic_arrays -lft -lft_dynarray -lreadline

LIBFT = libft/libft.a

DYNARRAYS = dynamic_arrays/libft_dynarray.a

BUILT_INS = ft_minishell_echo.c ft_minishell_pwd.c ft_minishell_env.c ft_minishell_split_cmd.c

LIBFT:
	make -C libft

DYNARRAYS:
	make -C dynamic_arrays

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

