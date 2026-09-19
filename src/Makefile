ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so
SYM_LINK = libft_malloc.so

CC = cc
CFLAGS = -fsanitize=address -g -Wall -Wextra -Werror

SRC = malloc.c free.c realloc.c show_alloc_mem.c
OBJ = $(SRC:.c=.o)
LIBFT_DIR = ../libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(NAME) $(SYM_LINK)

$(NAME): $(OBJ) $(LIBFT) Makefile
	$(CC) $(CFLAGS) $(OBJ) -L$(LIBFT_DIR) -I$(LIBFT_DIR) -lft -fPIC -shared -o $(NAME)

$(OBJ): malloc.h $(LIBFT_DIR)/libft.h $(LIBFT_DIR)/ft_printf.h

$(SYM_LINK): $(NAME)
	ln -sf $(NAME) $(SYM_LINK)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME) $(SYM_LINK)

re: fclean all

.PHONY: all clean fclean re
