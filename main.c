#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "./src/malloc.h"

// Assume your custom allocator functions are declared here:
// void *malloc(size_t size);
// void free(void *ptr);
// void *realloc(void *ptr, size_t size);

void test_null_and_zero(void) {
    printf("1. Testing NULL pointer and size 0... 🧪\n");

    // NULL pointer should act like malloc
    void *ptr = realloc(NULL, 64);
    assert(ptr != NULL);

    // Size 0 should act like free and return NULL
    void *ret = realloc(ptr, 0);
    assert(ret == NULL);

    printf("   PASSED! ✅\n\n");
}

void test_shrink_in_place(void) {
    printf("2. Testing in-place shrinking... 🧪\n");

    char *ptr = malloc(256);
    printf("%p\n", ptr);
    assert(ptr != NULL);
    
    // Fill payload with a pattern
    memset(ptr, 'A', 255);
    ptr[255] = '\0';

    // Shrink from 256 bytes down to 64 bytes
    char *new_ptr = realloc(ptr, 64);

    // Pointer address MUST remain identical for in-place shrink
    assert(new_ptr == ptr);

    // Verify payload data was preserved
    for (int i = 0; i < 63; i++) {
        assert(new_ptr[i] == 'A');
    }

    free(new_ptr);
    printf("   PASSED! ✅\n\n");
}

void test_expand_in_place(void) {
    printf("3. Testing in-place expansion (free neighbor)... 🧪\n");

    // Allocate two adjacent blocks
    char *block1 = malloc(64);
    char *block2 = malloc(64);
    assert(block1 != NULL && block2 != NULL);

    memset(block1, 'B', 63);
    block1[63] = '\0';

    // Free block2 so it becomes a free neighbor for block1
    free(block2);

    // Realloc block1 to absorb block2's space (up to 128 bytes)
    char *new_block1 = realloc(block1, 128);

    // Base pointer MUST remain identical because it expanded into the free neighbor
    assert(new_block1 == block1);

    // Verify existing payload data is intact
    for (int i = 0; i < 63; i++) {
        assert(new_block1[i] == 'B');
    }

    free(new_block1);
    printf("   PASSED! ✅\n\n");
}

void test_fallback_copy(void) {
    printf("4. Testing fallback allocation & copy... 🧪\n");

    // Allocate block1 and block2
    char *block1 = malloc(64);
    char *block2 = malloc(64); // Kept busy so block1 CANNOT expand in place

    memset(block1, 'C', 63);
    block1[63] = '\0';

    // Realloc block1 to a size that cannot fit in place (256 bytes)
    char *new_block1 = realloc(block1, 256);

    // New pointer MUST be different from the original pointer
    assert(new_block1 != block1);

    // Verify original data was correctly copied over to the new block
    for (int i = 0; i < 63; i++) {
        assert(new_block1[i] == 'C');
    }

    free(new_block1);
    free(block2);
    printf("   PASSED! ✅\n\n");
}

void test_heavy_churn_and_coalescing(void) {
    printf("5. Testing heavy allocation churn & coalescing... 🧪\n");

    void *ptrs[100];

    // Phase 1: Allocate 100 blocks
    for (int i = 0; i < 100; i++) {
        ptrs[i] = malloc(32 + (i % 4) * 16);
        assert(ptrs[i] != NULL);
    }

    // Phase 2: Free every EVEN index to create a "checkerboard" pattern of free blocks
    for (int i = 0; i < 100; i += 2) {
        free(ptrs[i]);
    }

    // Phase 3: Expand odd-indexed blocks in place into their freed neighbors
    for (int i = 1; i < 100; i += 2) {
        void *old_ptr = ptrs[i];
        // Request double size — should merge with freed ptrs[i-1] or ptrs[i+1]
        void *new_ptr = realloc(old_ptr, 96);
        assert(new_ptr == old_ptr); 
    }

    // Cleanup remaining blocks
    for (int i = 1; i < 100; i += 2) {
        free(ptrs[i]);
    }

    printf("   PASSED! ✅\n\n");
}

void test_exact_boundary_split(void) {
    printf("6. Testing exact boundary split threshold... 🧪\n");

    // Allocate 128 bytes
    char *ptr = malloc(128);
    assert(ptr != NULL);

    // Realloc down to a size that leaves EXACTLY not enough room for a header + MIN_ALLOC
    // e.g., leftover is sizeof(t_header) + MIN_ALLOC - 1
    size_t target_leftover = sizeof(t_header) + 16 - 1; // Assuming MIN_ALLOCATION_SIZE is 16
    size_t new_size = 128 - target_leftover;

    char *new_ptr = realloc(ptr, new_size);
    assert(new_ptr == ptr); // Must shrink in place WITHOUT splitting

    free(new_ptr);
    printf("   PASSED! ✅\n\n");
}
int main(void) {
    printf("========== RUNNING REALLOC TEST SUITE ==========\n\n");

    test_null_and_zero();
    test_shrink_in_place();
    test_expand_in_place();
    test_fallback_copy();
    test_heavy_churn_and_coalescing();
    test_exact_boundary_split();

    printf("================ ALL TESTS PASSED! 🎉 ================\n");
    return 0;
}