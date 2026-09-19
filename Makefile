ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = malloc_test

SRC = main.c

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
LIBFT_SRC = $(filter-out %_bonus.c,$(wildcard $(LIBFT_DIR)/*.c)) \
	$(wildcard $(LIBFT_DIR)/*.h)

MALLOC_DIR = src
MALLOC = $(MALLOC_DIR)/libft_malloc_$(HOSTTYPE).so
MALLOC_SRC = $(addprefix $(MALLOC_DIR)/,malloc.c free.c realloc.c \
	show_alloc_mem.c malloc.h)

all: $(NAME)

$(NAME): $(SRC) $(MALLOC) Makefile
	$(CC) $(SRC) $(CFLAGS) $(MALLOC) -o $(NAME)

$(LIBFT): $(LIBFT_SRC) $(LIBFT_DIR)/Makefile
	$(MAKE) -C $(LIBFT_DIR)

$(MALLOC): $(MALLOC_SRC) $(MALLOC_DIR)/Makefile $(LIBFT)
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

.PHONY: all test clean fclean re
