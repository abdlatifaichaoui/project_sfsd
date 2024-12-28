#include <stdio.h>
#include <string.h>

#define NUM_BLOCKS 100   // Total number of blocks
#define BLOCK_SIZE 512   // Size of each block (in bytes)

typedef struct {
    int is_occupied;     // 0 = Free, 1 = Occupied
    char data[BLOCK_SIZE];
} Block;

Block virtual_disk[NUM_BLOCKS]; // Virtual disk with blocks

// Initialize the memory allocation table
void initialize_allocation_table() {
    for (int i = 0; i < NUM_BLOCKS; i++) {
        virtual_disk[i].is_occupied = 0; // All blocks are free at initialization
        memset(virtual_disk[i].data, 0, BLOCK_SIZE); // Clear block data
    }
}

// Update the allocation table for a specific block
void update_allocation_table(int block_index, int status) {
    if (block_index >= 0 && block_index < NUM_BLOCKS) {
        virtual_disk[block_index].is_occupied = status;
    }
}

// Display the allocation table
void display_allocation_table() {
    printf("Block\tStatus\n");
    for (int i = 0; i < NUM_BLOCKS; i++) {
        printf("%d\t%s\n", i, virtual_disk[i].is_occupied ? "Occupied" : "Free");
    }
}

// Find the first free block
int find_free_block() {
    for (int i = 0; i < NUM_BLOCKS; i++) {
        if (virtual_disk[i].is_occupied == 0) {
            return i; // Return the index of the first free block
        }
    }
    return -1; // No free block found
}

// Allocate a block
void allocate_block(int block_index) {
    if (block_index >= 0 && block_index < NUM_BLOCKS) {
        virtual_disk[block_index].is_occupied = 1;
    }
}

// Free a block
void free_block(int block_index) {
    if (block_index >= 0 && block_index < NUM_BLOCKS) {
        virtual_disk[block_index].is_occupied = 0;
        memset(virtual_disk[block_index].data, 0, BLOCK_SIZE); // Clear block data
    }
}

// Compact the memory to eliminate fragmentation
void compact_memory() {
    int write_index = 0;

    for (int read_index = 0; read_index < NUM_BLOCKS; read_index++) {
        if (virtual_disk[read_index].is_occupied) {
            if (write_index != read_index) {
                // Move block data
                memcpy(&virtual_disk[write_index], &virtual_disk[read_index], sizeof(Block));
                virtual_disk[read_index].is_occupied = 0; // Mark old location as free
                memset(virtual_disk[read_index].data, 0, BLOCK_SIZE);
            }
            write_index++;
        }
    }
}

int main() {
    initialize_allocation_table();

    printf("Initial state of the allocation table:\n");
    display_allocation_table();

    // Test block allocation
    int block1 = find_free_block();
    allocate_block(block1);
    printf("\nAfter allocating block %d:\n", block1);
    display_allocation_table();

    int block2 = find_free_block();
    allocate_block(block2);
    printf("\nAfter allocating block %d:\n", block2);
    display_allocation_table();

    // Test freeing a block
    free_block(block1);
    printf("\nAfter freeing block %d:\n", block1);
    display_allocation_table();

    // Test memory compaction
    printf("\nBefore compaction:\n");
    display_allocation_table();
    compact_memory();
    printf("\nAfter compaction:\n");
    display_allocation_table();

    return 0;
}
