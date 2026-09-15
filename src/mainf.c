#include "malloc.h"
// Helper function to print a single block's state
void print_block(const char *label, t_header *block) {
    if (!block) {
        printf("  %-15s: NULL\n", label);
        return;
    }
    printf("  %-15s: [Addr: %p | Size: %4zu | Free: %s | Prev: %p | Next: %p]\n",
           label,
           (void*)block,
           block->size,
           block->is_free ? "YES" : " NO",
           (void*)block->prev,
           (void*)block->next);
}

// Helper to manually create contiguous headers in a dummy memory buffer
t_header *setup_dummy_block(void *buffer_addr, size_t size, bool is_free) {
    t_header *h = (t_header *)buffer_addr;
    h->size = size;
    h->is_free = is_free;
    h->next = NULL;
    h->prev = NULL;
    return h;
}

void run_coalesce_tests(void) {
    printf("===================================================\n");
    printf("       🧪 RUNNING free COALESCING TESTS        \n");
    printf("===================================================\n\n");

    // Create a 1KB local buffer simulating a TINY page region
    char memory_page[1024];

    // -------------------------------------------------------------
    // TEST 1: Forward Merge (Right Neighbor Free)
    // -------------------------------------------------------------
    printf("--- Test 1: Forward Merge (Freeing B when C is free) ---\n");
    
    // Layout: [ Block B (Busy, 64B) ] <-> [ Block C (Free, 128B) ]
    t_header *b1 = setup_dummy_block(memory_page, 64, false);
    t_header *c1 = setup_dummy_block(memory_page + sizeof(t_header) + 64, 128, true);
    
    b1->next = c1;
    c1->prev = b1;

    printf("BEFORE free(B):\n");
    print_block("Block B (ptr)", b1);
    print_block("Block C", c1);

    // Pass the payload address (after header)
    free((void*)b1 + sizeof(t_header));

    printf("AFTER free(B):\n");
    print_block("Merged Block B", b1);
    printf("Expected Size: %zu (64 + 128 + %zu)\n\n", 64 + 128 + sizeof(t_header), sizeof(t_header));

    // -------------------------------------------------------------
    // TEST 2: Backward Merge (Left Neighbor Free)
    // -------------------------------------------------------------
    printf("--- Test 2: Backward Merge (Freeing B when A is free) ---\n");
    
    // Layout: [ Block A (Free, 64B) ] <-> [ Block B (Busy, 64B) ]
    t_header *a2 = setup_dummy_block(memory_page, 64, true);
    t_header *b2 = setup_dummy_block(memory_page + sizeof(t_header) + 64, 64, false);

    a2->next = b2;
    b2->prev = a2;

    printf("BEFORE free(B):\n");
    print_block("Block A", a2);
    print_block("Block B (ptr)", b2);

    free((void*)b2 + sizeof(t_header));

    printf("AFTER free(B):\n");
    print_block("Merged Block A", a2);
    printf("Expected Size: %zu (64 + 64 + %zu)\n\n", 64 + 64 + sizeof(t_header), sizeof(t_header));

    // -------------------------------------------------------------
    // TEST 3: Double Coalesce (Both Neighbors Free)
    // -------------------------------------------------------------
    printf("--- Test 3: Double Coalesce (Freeing B when A & C are free) ---\n");
    
    // Layout: [ Block A (Free, 64B) ] <-> [ Block B (Busy, 32B) ] <-> [ Block C (Free, 128B) ]
    t_header *a3 = setup_dummy_block(memory_page, 64, true);
    t_header *b3 = setup_dummy_block(memory_page + sizeof(t_header) + 64, 32, false);
    t_header *c3 = setup_dummy_block((void*)b3 + sizeof(t_header) + 32, 128, true);

    a3->next = b3;
    b3->prev = a3;
    b3->next = c3;
    c3->prev = b3;

    printf("BEFORE free(B):\n");
    print_block("Block A", a3);
    print_block("Block B (ptr)", b3);
    print_block("Block C", c3);

    free((void*)b3 + sizeof(t_header));

    printf("AFTER free(B):\n");
    print_block("Merged Block A", a3);
    print_block("A->next", a3->next);
    printf("Expected Size: %zu (64 + 32 + 128 + 2*%zu)\n\n", 
           64 + 32 + 128 + (2 * sizeof(t_header)), sizeof(t_header));
}

int main(void) {
    run_coalesce_tests();
    return 0;
}