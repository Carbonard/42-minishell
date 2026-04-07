.PHONY: all clean fclean re

INCLUDE_DIR = ./includes

CC = cc
CFLAGS = -Wall -Werror -Wextra -I $(INCLUDE_DIR) -I ./libft -g3 # -fsanitize=address
CLIBS = -L./libft -L./dynamic_arrays -lft -lft_dynarray -lreadline

NAME = minishell
all: $(NAME)

LIBFT = libft/libft.a
DYNARRAYS = dynamic_arrays/libft_dynarray.a

BUILTINS = ft_minishell_builtins ft_minishell_builtins_others ft_minishell_builtins_env ft_minishell_exit ft_minishell_clears
INPUT = ft_minishell_get_input ft_minishell_heredocs ft_minishell_check_syntax ft_minishell_check_input
EXECUTION = ft_minishell_argv ft_minishell_parser ft_minishell_split_cmd ft_minishell_execute_tree ft_minishell_execute_node ft_minishell_execute_leaf ft_minishell_execute_command
OTHER = ft_minishell_main ft_minishell_environment ft_minishell_debug ft_minishell_error_messages ft_minishell_wildcards ft_minishell_wildcards_sort ft_minishell_utils

SRC_DIR = src
FILES = $(BUILTINS) $(INPUT) $(EXECUTION) $(OTHER)
SRC = $(FILES:%=$(SRC_DIR)/%.c)

OBJ_DIR = obj
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

print:
	@echo $(SRC)
	@echo '---------------------------------'
	@echo $(OBJ)

$(LIBFT):
	make -C libft

$(DYNARRAYS):
	make -C dynamic_arrays

$(OBJ_DIR):
	@mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/ft_minishell.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) $< -c -o $@

$(NAME): $(LIBFT) $(DYNARRAYS) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(CLIBS) -o $@

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all
