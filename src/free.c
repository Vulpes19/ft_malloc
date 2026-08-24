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