#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "./src/malloc.h"

void    test_tiny_zone(void) {
    printf("🧪 Starting Multiple Zone Test...\n\n");

    void *ptrs[150];

    for (int i = 0; i < 150; i++) {
        ptrs[i] = malloc(100);

        if (ptrs[i] == NULL) {
            printf("❌ Allocation %d failed!\n", i);
            return ;
        }

        // Print the address returned so we can inspect memory gaps!
        printf("Alloc %d: Payload address = %p\n", i, ptrs[i]);
    }

    printf("\n✅ Successfully allocated across multiple zones!\n");
}

void test_small_zone(void) {
    printf("\n====================================\n");
    printf("        TESTING SMALL ZONE          \n");
    printf("====================================\n");

    // Allocation size: 512 bytes (TINY <= 128, SMALL <= 1024)
    size_t alloc_size = 512;
    int num_allocs = 35; // 35 * ~512 bytes > 16384 (16KB page)
    void *ptrs[35];

    printf("--> Allocating %d blocks of %zu bytes...\n", num_allocs, alloc_size);

    for (int i = 0; i < num_allocs; i++) {
        ptrs[i] = malloc(alloc_size);

        if (ptrs[i] == NULL) {
            printf("❌ Alloc %d failed (NULL)\n", i);
            break;
        }

        // Fill memory with dummy data to ensure address is writable
        memset(ptrs[i], 0xAB, alloc_size);

        // Print key address steps to observe page boundary crossings
        if (i == 0 || i == 31 || i == 32) {
            printf("  Alloc %2d: ptr = %p\n", i, ptrs[i]);
        }
    }
}

void test_large_zone(void) {
    printf("\n====================================\n");
    printf("        TESTING LARGE ZONE          \n");
    printf("====================================\n");

    // Case 1: Just over SMALL limit (e.g., 2048 bytes)
    printf("--> Requesting 2048 bytes (Just over SMALL boundary)\n");
    void *p1 = malloc(2048);
    if (p1) memset(p1, 'A', 2048);
    printf("  p1 address = %p\n", p1);

    // Case 2: Multi-page LARGE allocation (e.g., 50,000 bytes > 16KB page)
    printf("--> Requesting 50000 bytes (Multi-page request)\n");
    void *p2 = malloc(50000);
    if (p2) memset(p2, 'B', 50000);
    printf("  p2 address = %p\n", p2);
}

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
    // printf("========== RUNNING REALLOC TEST SUITE ==========\n\n");

    // test_null_and_zero();
    // test_shrink_in_place();
    // test_expand_in_place();
    // test_fallback_copy();
    // test_heavy_churn_and_coalescing();
    // test_exact_boundary_split();

    // printf("================ ALL TESTS PASSED! 🎉 ================\n");
    // printf("========== RUNNING MALLOC TEST SUITE ==========\n\n");
    // test_tiny_zone();
    // show_alloc_mem();
    // printf("================ ALL TESTS PASSED! 🎉 ================\n");

    // printf("========= SMALL TESTS ===========\n");
    test_small_zone();
    test_large_zone();
    show_alloc_mem();

    return 0;
}