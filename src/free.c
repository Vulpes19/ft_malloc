#include "malloc.h"

// Day 6: Basic free() Implementation 🔓

// Implement pointer math in free(ptr) to find the Header sitting right before ptr.

// Set is_free = true.

// Day 7: LARGE Allocation Freeing 🐘

// Handle the simple case first: If the block is LARGE, call munmap() directly to return it to the OS immediately.

// Day 8: Coalescing (Merging) Block Logic 🤝

// Write a function to inspect adjacent blocks in a TINY/SMALL zone.

// If the next neighbor is free, merge their sizes into one single Header.

// Day 9: Zone Deallocation (munmap()) 🗑️

// Check if an entire TINY or SMALL zone becomes 100% free, and use munmap() to return the whole zone to the kernel.

// Day 10: Testing malloc() + free() Cycle 🧪

// Run torture tests: Repeatedly allocate and free memory blocks to ensure no memory leaks or segmentation faults occur.

void    merge_neighbors(t_header *curr_n, t_header *curr_p, t_header *header_ptr) {

    // check immediate neighbors
    if (curr_n && curr_n->is_free == true) {
        if (curr_n->next)
            curr_n->next->prev = header_ptr;
        header_ptr->next = curr_n->next;
        header_ptr->size += curr_n->size + + sizeof(t_header);
    }
    if (curr_p && curr_p->is_free == true) {
        header_ptr->next->prev = curr_p;
        curr_p->size += header_ptr->size + sizeof(t_header);
        curr_p->next = header_ptr->next;
        if (header_ptr->next) {
        }
    }

    // check if the whole zone is free
    // if (header_ptr->size == allocator.tiny_zone_size && zone == TINY) {
    //     int res = munmap(allocator.tiny, allocator.tiny_zone_size);
    //     if (res == -1) {
    //         fprintf(stderr, "mmap failed: %s\n", strerror(errno));
    //         exit(1);
    //     }
    // }
    // if (header_ptr->size == allocator.small_zone_size && zone == TINY) {
    //     int res = munmap(allocator.small, allocator.small_zone_size);
    //     if (res == -1) {
    //         fprintf(stderr, "mmap failed: %s\n", strerror(errno));
    //         exit(1);
    //     }
    // }
}

void    ft_free(void *ptr) {
    if (!ptr)
        return ;
    
    t_header *header = (void*)ptr - sizeof(t_header);
    size_t m = 1024;
    size_t size = header->size;
    
    if (size > m) {
        int res = munmap(header, size);
        if (res == -1) {
            fprintf(stderr, "mmap failed: %s\n", strerror(errno));
            return ;
        }
        return ;
    }
    
    if (header->is_free == false) {
        header->is_free = true;
        merge_neighbors(header->next, header->prev, header);
    }

    return ;
}