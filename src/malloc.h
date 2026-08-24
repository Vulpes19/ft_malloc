#pragma once

#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

enum ZONE {
    TINY,
    SMALL,
    LARGE
};

typedef struct s_header {
    size_t size;
    bool is_free;
    t_header *next;
} t_header;

typedef struct s_allocator {
    t_header *tiny;
    t_header *small;
    t_header *large;
} t_allocator;

t_allocator *allocator = {NULL, NULL, NULL};

void    *malloc(size_t size);