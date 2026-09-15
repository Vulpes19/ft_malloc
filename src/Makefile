ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so
SYM_LINK = libft_malloc.so

CC = cc
CFLAGS = -fsanitize=address -g -Wall -Wextra -Werror

SRC = malloc.c free.c realloc.c
OBJ = $(SRC:.c=.o)
LIBFT_DIR = ../Libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(NAME) $(SYM_LINK)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) -L$(LIBFT_DIR) -I$(LIBFT_DIR) -lft -fPIC -shared -o $(NAME)

$(SYM_LINK): $(NAME)
	ln -sf $(SYM_LINK) $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all