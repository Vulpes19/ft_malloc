#include "malloc.h"

void    *allocate_new_region(t_header *head_ptr, size_t new_size) {
    void *ret = malloc(new_size);
    if (!ret)
        return NULL;
    
    ret = ft_memcpy(ret, (void*)head_ptr + sizeof(t_header), (head_ptr->size > new_size) ? new_size : head_ptr->size);
    free((void *)head_ptr + sizeof(t_header));
    
    return ret;
}

void    *check_neighbor(t_header *head_ptr, size_t new_size) {
    t_header *next = head_ptr->next;

    printf("head_ptr->size: %zu *** new_size: %zu\n", head_ptr->size, new_size );
    if (head_ptr->size >= new_size) {
        printf("********* size >= new_size ********\n");
        size_t old_size = head_ptr->size;
        head_ptr->size = new_size;

        if (old_size - new_size >= sizeof(t_header) + MIN_ALLOCATION_SIZE) {
            printf("old size - new_size >= sizeof(t_header) + MIN_ALLOC\n");
            printf("%zu\n", old_size - new_size);
            t_header *block_end_ptr = (void *)head_ptr + sizeof(t_header) + new_size;
            
            block_end_ptr->size = old_size - new_size - sizeof(t_header);
            block_end_ptr->is_free = true;

            block_end_ptr->next = head_ptr->next;
            block_end_ptr->prev = head_ptr;
            head_ptr->next = block_end_ptr;

           if (head_ptr->next)
                head_ptr->next->prev = block_end_ptr;
        }
        return (void *)head_ptr + sizeof(t_header);
    }
    else if (next && next->is_free == true && next->size >= new_size - head_ptr->size) {
        printf("next && next->is_free == true && next->size >= new_size - head_ptr->size\n");

        head_ptr->size = new_size;
        head_ptr->next = next->next;
        
        if (next->next)
            next->next->prev = head_ptr;
        
        return (void *)head_ptr + sizeof(t_header);
    }
    else {
        printf("****** new region case *******\n");
        return allocate_new_region(head_ptr, new_size);
    }
}

void    *realloc(void *ptr, size_t size) {
    printf("%p\n", ptr);
    if (!ptr) {
        printf("***** malloc case *****\n");
        return malloc(size);
    }
    
    if (size == 0) {
        free(ptr);
        return NULL;
    }

    t_header *head_ptr = (t_header *)((char *)ptr - sizeof(t_header));

    return check_neighbor(head_ptr, size);
}