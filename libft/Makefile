.PHONY: all, clean, fclean, re, bonus

CFLAGS = -Wall -Werror -Wextra
#-fsanitize=address

NAME = libft.a

DIR_OBJ = build

DIR_SRC = src

FILES = ft_atoi ft_bzero ft_calloc ft_isalnum ft_isalpha\
ft_isascii ft_isdigit ft_isprint ft_itoa ft_memchr ft_memcmp\
ft_memcpy ft_memmove ft_memset ft_putchar_fd ft_putendl_fd\
ft_putnbr_fd ft_putstr_fd ft_split ft_strchr ft_strdup ft_striteri\
ft_strjoin ft_strlcat ft_strlcpy ft_strlen ft_strmapi ft_strncmp\
ft_strnstr ft_strrchr ft_strtrim ft_substr ft_tolower ft_toupper\
ft_lstadd_back_bonus ft_lstadd_front_bonus ft_lstclear_bonus\
ft_lstdelone_bonus ft_lstiter_bonus ft_lstlast_bonus ft_lstmap_bonus\
ft_lstnew_bonus ft_lstsize_bonus\
ft_utoa_base ft_atoi_base ft_basic_math ft_str_malloc\
ft_printf ft_printf_flags ft_printf_numbers_1 ft_printf_numbers_2 ft_printf_texts\
get_next_line

SRC = $(FILES:%=$(DIR_SRC)/%.c)
OBJ = $(FILES:%=$(DIR_OBJ)/%.o)

HEADER = libft.h

all: $(NAME)

$(NAME): $(OBJ)
	@ar rcs $@ $^

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	@mkdir -p $(DIR_OBJ)
	@cc -c $(CFLAGS) $^ -o $@

clean:
	@rm -rf $(DIR_OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all
