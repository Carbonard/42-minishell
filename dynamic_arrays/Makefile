.PHONY: all, clean, fclean, re

CFLAGS = -Wall -Werror -Wextra

NAME = libft_dynarray.a

FILES = dynarray_realloc dynamic_ptr dynamic_int

SRC_DIR = src

SRC = $(FILES:%=$(SRC_DIR)/ft_%.c)

OBJ_DIR = build

OBJ = $(FILES:%=$(OBJ_DIR)/ft_%.o)

all: $(NAME)

$(NAME): $(OBJ)
	@ar rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@cc -c $(CFLAGS) $^ -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all
