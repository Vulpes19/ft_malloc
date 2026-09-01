#include "malloc.h"
#include <errno.h>

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

t_allocator allocator = { NULL, 0, NULL, 0, NULL };

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

void    *split_block(size_t total_size, t_header *zone_ptr, size_t zone_size) {
    // void *zone_end_ptr = (void*)zone_ptr + zone_size;

    t_header *head = zone_ptr;

    while (head) {
        if (head->is_free == true) {
            size_t remaining_size = zone_size - total_size;

            if (remaining_size >= sizeof(t_header) + MIN_ALLOCATION_SIZE) {
                head->is_free = false;
                head->size = total_size;
                printf("is_free: false\nhead->size: %zu\n", head->size);
    
                t_header *block_end_ptr = (t_header *)((void*)head + total_size);
    
                block_end_ptr->is_free = true;
                block_end_ptr->next = NULL;
                block_end_ptr->size = remaining_size;
                printf("is_free: true\nblock end ptr: %zu\n", remaining_size);
    
                head->next = block_end_ptr;
            }
            else {
                return NULL;
            }
            
            return (void *)head + sizeof(t_header);
        }
        head = head->next;
    }

    return NULL;
}

void init_zone(size_t size, enum ZONE zone) {
    size_t page_size = 0;

    #ifdef __APPLE__
        page_size = getpagesize();
    #else
        page_size = sysconf(_SC_PAGESIZE);
    #endif

    if (zone == TINY) {
        size_t zone_size = calculate_zone_size(size * 100, page_size);
        allocator.tiny = mmap(NULL, zone_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
         if (allocator.tiny == MAP_FAILED) {
            fprintf(stderr, "mmap failed: %s\n", strerror(errno));
            exit(1);
        }
        allocator.tiny_zone_size = zone_size;
        allocator.tiny->is_free = true;
        allocator.tiny->size = zone_size;
        allocator.tiny->next = NULL;
    }
    else if (zone == SMALL) {
        printf("here\n");
        size_t zone_size = calculate_zone_size(size * 100, page_size);
        allocator.small = mmap(NULL, zone_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (allocator.small == MAP_FAILED) {
            fprintf(stderr, "mmap failed: %s\n", strerror(errno));
            exit(1);
        }
        allocator.small_zone_size = zone_size;
        allocator.small->is_free = true;
        allocator.small->size = zone_size;
        allocator.small->next = NULL;
    }
    else {
        allocator.large = mmap(NULL, calculate_zone_size(size, page_size), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (allocator.large == MAP_FAILED) {
            fprintf(stderr, "mmap failed: %s\n", strerror(errno));
            exit(1);
        }
        allocator.small->is_free = true;
        allocator.small->size = size;
        allocator.small->next = NULL;
    }
}

void    *ft_malloc(size_t size) {
    size_t n = 128;
    size_t m = 1024;
    size_t total_size = size + sizeof(t_header);
    void *res_ptr = NULL;
    printf("size: %zu \n", size);

    if (size <= n) {
        printf("TINY\n");
        if (allocator.tiny == NULL)
            init_zone(total_size, TINY);
        res_ptr = split_block(total_size, allocator.tiny, allocator.tiny_zone_size);
        return res_ptr;
    }
    else if (size > n && size <= m) {
        printf("SMALL\n");
        if (allocator.small == NULL)
            init_zone(total_size, SMALL);
        res_ptr = split_block(total_size, allocator.small, allocator.small_zone_size);
        return res_ptr;
    }
    else if (size > m) {
        printf("LARGE\n");
        init_zone(total_size, LARGE);
        res_ptr = split_block(total_size, allocator.small, allocator.small_zone_size);
        return res_ptr;
    }

    return NULL;
}