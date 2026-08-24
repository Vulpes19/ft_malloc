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