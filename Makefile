.PHONY: all clean fclean

CC = cc

CFLAGS = -Wall -Werror -Wextra -g3 -fsanitize=address

CLIBS = -L./libft -L./dynamic_arrays -lft -lft_dynarray -lreadline

NAME = minishell

LIBFT = libft/libft.a

DYNARRAYS = dynamic_arrays/libft_dynarray.a

BUILT_INS = ft_minishell_echo.c ft_minishell_pwd.c ft_minishell_env.c

SRC = ft_minishell_main.c ft_minishell_heredocs.c ft_minishell_check_syntax.c ft_minishell_check_input.c ft_minishell_environment.c ft_minishell_get_input.c ft_minishell_argv.c ft_minishell_parser.c ft_minishell_split_cmd.c ft_minishell_builtins.c ft_minishell_execute_tree.c ft_minishell_execute_command.c ft_minishell_exit.c ft_minishell_clears.c ft_minishell_debug.c ft_minishell_signals.c

$(LIBFT):
	make -C libft

$(DYNARRAYS):
	make -C dynamic_arrays

# echo: ft_minishell_echo.c $(LIBFT)
# 	$(CC) $(CFLAGS) $^ $(CLIBS) -o echo

# pwd: ft_minishell_pwd.c $(LIBFT)
# 	$(CC) $(CFLAGS) $^ $(CLIBS) -o pwd

# env: ft_minishell_env.c $(LIBFT)
# 	$(CC) $(CFLAGS) $^ $(CLIBS) -o env

# cd: ft_minishell_cd.c $(LIBFT)
# 	$(CC) $(CFLAGS) $^ $(CLIBS) -o cd

$(NAME): $(LIBFT) $(DYNARRAYS) $(BUILT_INS) $(SRC)
	$(CC) $(CFLAGS) $^ $(CLIBS) -o $@

clean:
	@rm -f $(NAME)

re: clean $(NAME)