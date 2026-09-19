#include "malloc.h"

void    show_alloc_mem(void) {
    if (allocator.tiny) {
        t_header *curr = allocator.tiny;
        ft_printf("TINY : %p\n", curr);
        while (curr) {
            if (!curr->is_free) {
                void *start = (void*)curr + sizeof(t_header);
                void *end = start + curr->size;
                ft_printf("%p - %p : %u bytes\n", start, end, curr->size);
            }
            curr = curr->next;
        }
        ft_printf("Total : %u\n", allocator.tiny_zone_size);
    }
    if (allocator.small) {
        t_header *curr = allocator.small;
        ft_printf("SMALL : %p\n", curr);
        while (curr) {
            if (!curr->is_free) {
                void *start = (void*)curr + sizeof(t_header);
                void *end = start + curr->size;
                ft_printf("%p - %p : %u bytes\n", start, end, curr->size);
            }
            curr = curr->next;
        }
        ft_printf("Total : %u\n", allocator.small_zone_size);
    }
    if (allocator.large) {
        t_header *curr = allocator.large;
        ft_printf("LARGE : %p\n", curr);
        size_t total = 0;
        while (curr) {
            if (!curr->is_free) {
                void *start = (void*)curr + sizeof(t_header);
                void *end = start + curr->size;
                total += curr->size;
                ft_printf("%p - %p : %u bytes\n", start, end, (unsigned int)curr->size);
            }
            curr = curr->next;
        }
        ft_printf("Total : %u\n", total);
    }
}