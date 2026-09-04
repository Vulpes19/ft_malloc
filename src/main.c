#include <unistd.h>
#include <stdio.h>
#include "malloc.h"

void    test_tiny_zone(void) {
    printf("🧪 Starting Multiple Zone Test...\n\n");

    void *ptrs[150];

    for (int i = 0; i < 150; i++) {
        ptrs[i] = ft_malloc(100);

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
        ptrs[i] = ft_malloc(alloc_size);

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
    void *p1 = ft_malloc(2048);
    if (p1) memset(p1, 'A', 2048);
    printf("  p1 address = %p\n", p1);

    // Case 2: Multi-page LARGE allocation (e.g., 50,000 bytes > 16KB page)
    printf("--> Requesting 50000 bytes (Multi-page request)\n");
    void *p2 = ft_malloc(50000);
    if (p2) memset(p2, 'B', 50000);
    printf("  p2 address = %p\n", p2);
}

int main() {
    // char *ptr1 = ft_malloc(200); // First SMALL allocation
    // char *ptr2 = ft_malloc(300); // Second SMALL allocation
    // char *ptr3 = ft_malloc(6000); // Third SMALL allocation
    // char *ptr4 = ft_malloc(6000); // Fourth SMALL allocation

    // printf("ptr1 address: %p\n", (void *)ptr1);
    // printf("ptr2 address: %p\n", (void *)ptr2);
    // printf("ptr3 address: %p\n", (void *)ptr3);
    // printf("ptr4 address: %p\n", (void *)ptr4);
    
    
    // test_small_zone();
    test_small_zone();
    return 0;
}