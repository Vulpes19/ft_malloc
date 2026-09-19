#include "malloc.h"
#include <errno.h>

// TINY zone = 16384 / n = 128
// SMALL zone = 114688 / n + 1 to m = 1024

t_allocator allocator = { NULL, 0, NULL, 0, NULL };

int calculate_zone_size(size_t size, size_t page_size) {
    size_t new_page_size = page_size;
    if (size > page_size) {
        size_t f = (size + page_size - 1) / page_size;
        new_page_size = page_size * f;
    }

    return new_page_size;
}

void    *split_block(size_t total_size, t_header *zone_ptr, size_t zone_size) {
    void *zone_end_ptr = (void*)zone_ptr + zone_size;

    t_header *head = zone_ptr;

    while (head) {
        if (head->is_free == true) {
            size_t remaining_size = (head->size >= total_size) ? head->size - total_size : total_size - head->size;

            void *block_end_addr = (void *)head + total_size;

            if (block_end_addr + sizeof(t_header) + 40 <= zone_end_ptr) {
                head->is_free = false;
                head->size = total_size;
                
                t_header *block_end_ptr = (t_header *)((void*)head + total_size);
    
                block_end_ptr->is_free = true;
                block_end_ptr->next = NULL;
                block_end_ptr->size = remaining_size;
    
                head->next = block_end_ptr;
                block_end_ptr->prev = head;
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


void    *allocate_new_zone_region(size_t total_size, t_header *zone_ptr, size_t zone_size) {
    t_header *curr = zone_ptr;
    while (curr->next) {
        curr = curr->next;
    }
    t_header *zone_end_ptr = curr;
    t_header *new_region = mmap(NULL, zone_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (new_region == MAP_FAILED) {
        return NULL;
    }
    new_region->is_free = true;
    new_region->next = NULL;
    new_region->size = zone_size;

    t_header *header_end_ptr = zone_end_ptr;

    header_end_ptr->next = new_region;
    new_region->prev = header_end_ptr;
    zone_ptr->size = zone_size + new_region->size;

    void *res_ptr = split_block(total_size, zone_ptr, zone_ptr->size);
    return res_ptr;
}

int init_zone(size_t size, enum ZONE zone) {
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
            return EXIT_FAILURE;
        }
        allocator.tiny_zone_size = zone_size;
        allocator.tiny->is_free = true;
        allocator.tiny->size = zone_size;
        allocator.tiny->next = NULL;
        allocator.tiny->prev = NULL;
    }
    else if (zone == SMALL) {
        size_t zone_size = calculate_zone_size(size * 100, page_size);
        allocator.small = mmap(NULL, zone_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (allocator.small == MAP_FAILED) {
            return EXIT_FAILURE;
        }
        allocator.small_zone_size = zone_size;
        allocator.small->is_free = true;
        allocator.small->size = zone_size;
        allocator.small->next = NULL;
        allocator.small->prev = NULL;
    }
    else {
        size_t large_size = calculate_zone_size(size, page_size);
        t_header *new_large_zone = mmap(NULL, large_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

        if (new_large_zone == MAP_FAILED) {
            return EXIT_FAILURE;
        }

        new_large_zone->is_free = true;
        new_large_zone->size = large_size;
        new_large_zone->next = NULL;
        new_large_zone->prev = NULL;

        if (allocator.large == NULL) {
            allocator.large = new_large_zone;
        }
        else {
            t_header    *head = allocator.large;
            while (head->next) {
                head = head->next;
            }
            head->next = new_large_zone;
            new_large_zone->prev = head;
        }
    }
    return EXIT_SUCCESS;
}

void    *malloc(size_t size) {
    size_t n = 128;
    size_t m = 1024;
    size_t total_size = size + sizeof(t_header);
    void *res_ptr = NULL;

    if (size <= n) {
        if (allocator.tiny == NULL) {
            if (init_zone(total_size, TINY) == EXIT_FAILURE)
                return NULL;
        }
        res_ptr = split_block(total_size, allocator.tiny, allocator.tiny_zone_size);
        if (res_ptr == NULL) {
            res_ptr = allocate_new_zone_region(total_size, allocator.tiny, allocator.tiny_zone_size);
        }
        return res_ptr;
    }
    else if (size > n && size <= m) {
        if (allocator.small == NULL)
            if (init_zone(total_size, SMALL) == EXIT_FAILURE)
                return NULL;
        res_ptr = split_block(total_size, allocator.small, allocator.small_zone_size);
        if (res_ptr == NULL) {
            res_ptr = allocate_new_zone_region(total_size, allocator.small, allocator.small_zone_size);
        }
        return res_ptr;
    }
    else if (size > m) {
        if (init_zone(total_size, LARGE) == EXIT_FAILURE)
                return NULL;
        t_header *head = allocator.large;
        while (head) {
            if (head->is_free == true) {
                head->is_free = false;
                res_ptr = (void*)head;
            }
            head = head->next;
        }
        return res_ptr + sizeof(t_header);
    }

    return NULL;
}