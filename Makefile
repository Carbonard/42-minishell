.PHONY: all clean fclean re force

INCLUDE_DIR = ./includes

CC = cc
CFLAGS = -Wall -Werror -Wextra -I $(INCLUDE_DIR) -I ./libft -g3 #-fsanitize=address
CLIBS = -L./libft -L./dynamic_arrays -lft -lft_dynarray -lreadline

NAME = minishell
all: $(NAME)

LIBFT = libft/libft.a
DYNARRAYS = dynamic_arrays/libft_dynarray.a

BUILTINS = builtins builtins_others builtins_env exit clears
INPUT = get_input heredocs check_syntax check_input
EXECUTION = argv parser split_cmd execute_tree execute_node execute_leaf execute_command
OTHER = main environment debug error_messages wildcards wildcards_sort utils signals history init_config

SRC_DIR = src
FILES = $(BUILTINS) $(INPUT) $(EXECUTION) $(OTHER)
SRC = $(FILES:%=$(SRC_DIR)/ft_minishell_%.c)

OBJ_DIR = obj
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

print:
	@echo $(SRC)
	@echo '---------------------------------'
	@echo $(OBJ)

$(LIBFT): force
	@make -C libft

$(DYNARRAYS): force
	@make -C dynamic_arrays

$(OBJ_DIR):
	@mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/ft_minishell.h | $(OBJ_DIR)
	@$(CC) $(CFLAGS) $< -c -o $@

$(NAME): $(LIBFT) $(DYNARRAYS) $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) $(CLIBS) -o $@

bash: $(NAME)
	@mv $(NAME) bash

test: bash
	@echo "Use the following command:"
	@echo 'source ./tester.sh'

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all
