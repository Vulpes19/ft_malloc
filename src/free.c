#include "malloc.h"

void    merge_neighbors(t_header *curr_n, t_header *curr_p, t_header *header_ptr) {

    // check immediate neighbors
    if (curr_n && curr_n->is_free == true) {
        if (curr_n->next)
            curr_n->next->prev = header_ptr;
        header_ptr->next = curr_n->next;
        header_ptr->size += curr_n->size + + sizeof(t_header);
    }
    if (curr_p && curr_p->is_free == true) {
        if (header_ptr->next)
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

void    free(void *ptr) {
    if (!ptr)
        return ;
    
    t_header *header = (t_header *)((char*)ptr - sizeof(t_header));
    if (!header || (header && header->is_free))
        return ;
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