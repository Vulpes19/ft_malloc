#include "malloc.h"

// Day 11: Simple realloc() Case 🔄

// Implement edge cases for realloc(ptr, size): realloc(NULL, size) acts like malloc(), and realloc(ptr, 0) acts like free().

// Day 12: In-Place realloc() Expansion 📈

// Check if the current block (or its free next neighbor) already has enough space for the new size without moving data.

// Day 13: Full realloc() (Copy & Move) 📋

// If expanding in-place isn't possible: malloc() new space, memcpy() the old data over, and free() the old pointer.

// Day 14: Alignment & Edge Cases 🎯

// Ensure all returned memory addresses are strictly aligned (usually 8-byte or 16-byte alignment).

// Day 15: Final Testing & Code Cleanup 🚀

// Refactor code, remove debug prints, and test edge cases (allocating 0 bytes, extremely large requests, etc.).

void    *allocate_new_region(t_header *head_ptr, size_t new_size) {
    void *ret = malloc(new_size);
    if (!ret)
        return NULL;
    
    ret = ft_memcpy(ret, (void*)head_ptr + sizeof(t_header), (head_ptr->size > new_size) ? new_size : head_ptr->size);
    free(head_ptr);
    
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