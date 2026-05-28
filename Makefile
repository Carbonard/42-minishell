.PHONY: all clean fclean re force bonus

INCLUDE_DIR = ./includes

CC = cc
CFLAGS = -Wall -Werror -Wextra -I $(INCLUDE_DIR) -I ./libft -g3 #-fsanitize=address
CLIBS = -L./libft -L./dynamic_arrays -lft -lft_dynarray -lreadline

NAME = minishell
all: $(NAME)

LIBFT = libft/libft.a
DYNARRAYS = dynamic_arrays/libft_dynarray.a

BUILTINS = builtins builtins_others builtins_env exit clears builtins_echo
INPUT = get_input read_heredocs heredocs check_syntax check_input prompt escape tokens manage_input
EXECUTION = redirections expand_vars tree execute_tree execute_node execute_leaf execute_command
OTHER = main environment debug error_messages wildcards wildcards_sort utils signals history init_config

SRC_DIR = src
FILES = $(BUILTINS) $(INPUT) $(EXECUTION) $(OTHER)
SRC = $(FILES:%=$(SRC_DIR)/ft_minishell_%.c)

OBJ_DIR = obj
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

$(LIBFT): force
	make -C libft

$(DYNARRAYS): force
	make -C dynamic_arrays

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/ft_minishell.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) $< -c -o $@

$(NAME): $(LIBFT) $(DYNARRAYS) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(CLIBS) -o $@

bonus: $(NAME)

test: $(NAME)
	@echo "Use the following command:"
	@echo 'source ./tester.sh'

clean:
	make clean -C libft
	make clean -C dynamic_arrays
	rm -rf $(OBJ_DIR)

fclean: clean
	make fclean -C libft
	make fclean -C dynamic_arrays
	rm -f $(NAME)

re: fclean all

