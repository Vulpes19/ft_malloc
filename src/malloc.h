#pragma once

#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>

enum ZONE {
    TINY,
    SMALL,
    LARGE
};

typedef struct s_header {
    size_t size;
    bool is_free;
    struct s_header *next;
} t_header;

typedef struct s_allocator {
    t_header *tiny;
    t_header *small;
    t_header *large;
} t_allocator;

extern t_allocator allocator;
// extern keyword tells the compiler this is declaration only, no allocation

void    *ft_malloc(size_t size);