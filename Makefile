ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

CC = cc
CFLAGS = -fsanitize=address -g -Wall -Wextra -Werror
NAME = malloc_test

SRC = main.c

LIBFT_DIR = Libft
LIBFT = $(LIBFT_DIR)/libft.a

MALLOC_DIR = src
MALLOC = $(MALLOC_DIR)/libft_malloc_$(HOSTTYPE).so

all: $(NAME)

$(NAME): $(LIBFT) $(MALLOC)
	$(CC) main.c $(CFLAGS) $(MALLOC) -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(MALLOC):
	$(MAKE) -C $(MALLOC_DIR)

test: $(NAME)
	DYLD_INSERT_LIBRARIES=$(MALLOC) DYLD_FORCE_FLAT_NAMESPACE=1 ./$(NAME)

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	$(MAKE) -C $(MALLOC_DIR) clean

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(MAKE) -C $(MALLOC_DIR) fclean
	rm -f $(NAME)

re: fclean all