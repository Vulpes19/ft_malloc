#include "malloc.h"

// TINY zone = 16384 / n = 128
// SMALL zone = 114688 / n + 1 to m = 1024


// Day 1: Data Structures 📐

// Define your Header struct in C (fields for size, is_free, next).

// Define your global zone tracking variables (pointers to the head of TINY, SMALL, and LARGE lists).

// Day 2: Page Math & Zone Initialization 🧮

// Write helper functions to calculate page-aligned sizes using getpagesize().

// Write a function to request a new zone from the OS using mmap().

// Day 3: Block Splitting Logic ✂️

// Write a helper function that takes a large free block and splits it into an allocated block + a remaining free block.

// Day 4: Implement malloc() Logic 🏗️

// Implement the decision logic: Determine if a request is TINY, SMALL, or LARGE.

// Search existing zones for a free block or allocate a new zone if full.

// Day 5: Testing malloc() 🧪

// Write test scripts to make multiple calls to malloc() of varying sizes and verify the returned pointers are valid and aligned.

t_allocator allocator = { NULL, NULL, NULL };

int calculate_zone_size(size_t size, size_t page_size) {
    size_t new_page_size = page_size;
    printf("total size: %zu\n", size);
    if (size > page_size) {
        size_t f = (size + page_size - 1) / page_size;
        new_page_size = page_size * f;
    }

    printf("new page size: %zu\n", new_page_size);
    return new_page_size;
}

void init_zone(size_t size, enum ZONE zone) {
    size_t page_size = 0;

    #ifdef __APPLE__
        page_size = getpagesize();
    #else
        page_size = sysconf(_SC_PAGESIZE);
    #endif

    if (zone == TINY) {
        allocator.tiny = mmap(NULL, calculate_zone_size(size * 100, page_size), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    }
    else if (zone == SMALL) {
        allocator.small = mmap(NULL, calculate_zone_size(size * 100, page_size), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    }
    else {
        allocator.large = mmap(NULL, calculate_zone_size(size, page_size), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    }
}

void    *ft_malloc(size_t size) {
    size_t n = 128;
    size_t m = 1024;
    size_t total_size = size + sizeof(t_header);
    printf("total size: %zu\n", total_size);

    if (allocator.tiny == NULL && size <= n) {
        printf("TINY\n");
        init_zone(total_size, TINY);
    }
    else if (allocator.small == NULL && size > n && size <= m) {
        printf("SMALL\n");
        init_zone(total_size, SMALL);
    }
    else {
        printf("LARGE\n");
        init_zone(total_size, LARGE);
    }
    
    return NULL;
}