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

void *init_zone(size_t size, enum ZONE zone) {
    int page_size = 0;

    #ifdef __APPLE__
        page_size = getpagesize();
    #else
        page_size = sysconf(_SC_PAGESIZE);
    #endif

    
}

void    *malloc(size_t size) {
    size_t n = 128;
    size_t m = 1024;
    void *ptr = NULL;

    if (allocator->tiny == NULL && size <= 128)
        init_zone(size, TINY);
}