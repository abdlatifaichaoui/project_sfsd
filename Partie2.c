#include <stdio.h>
#include <stdling.h>
#include <string.h>

#define MAX_FFILE 10
#define MAX_SIZE 10

// Structure to represent a record:
typedef struct {
    int id;
    char name[50];
} Record;

//  BLOCK:
// For the chained organization:
typedef struct Block {
    Record record;               
    struct Block *next;        
} Block;

//organisation contigue:
typedef struct {
    Record records[10];        
    int current_size;         
} ContiguousFile;

// FILE:
// Chained:
typedef struct {
    Block *head;              
} ChainedFile;

// Contiguous:
typedef struct {
    Record records[10];         
    int max_size;               
    int current_size;           
} ContiguousFile;

//MS:
typedef struct {
    ChainedFile chained_files[MAX_FILES];      
    ContiguousFile contiguous_files[MAX_FILES]; 
    int number_of_files;
} SecondaryMemory;                        

// Contiguous organization:
void initializeContiguousFile(ContiguousFile *file) {
    file->current_size = 0; 
}
int add_record_contiguous(ContiguousFile *file, int id, const char *name) {
    if (file->current_size >= MAX_SIZE) {
        printf("Error: Insufficient space in the contiguous file.\n");
        return 0; // Failure
    }
    file->records[file->current_size].id = id; 
    strcpy(file->records[file->current_size].name, name);
    file->current_size++; 
    return 1; // Success
}
void free_contiguous_file(ContiguousFile *file) {
    file->current_size = 0;
}

void display_contiguous_file(const ContiguousFile *file) {
    if (file->current_size == 0) {
        printf("The file is empty.\n");
    }
    for (int i = 0; i < file->current_size; i++) {
        printf("Record %d: ID=%d, Name=%s\n", i + 1, file->records[i].id, file->records[i].name);
    }
}
void compact_contiguous_file(ContiguousFile *file) {
    int i, j = 0;
    for (i = 0; i < file->current_size; i++) {
        if (file->records[i].id != 0) { 
            file->records[j] = file->records[i]; 
            j++;
        }
    }
    file->current_size = j; 
}
void defragment_contiguous_file(ContiguousFile *file) {
    int j = 0;
    for (int i = 0; i < file->current_size; i++) {
        if (file->records[i].id != 0) { 
            file->records[j] = file->records[i]; 
            j++;
        }
    }
    file->current_size = j; 
}

// Chained organization:
void initialize_chained_file(ChainedFile *file) {
    file->head = NULL; 
}
void add_record_chained(ChainedFile *file, int id, const char *name) {
    Block *new_block = (Block *)malloc(sizeof(Block)); // Allocate memory for a new block
    if (new_block == NULL) {
        printf("Error: Insufficient memory to allocate a new block.\n");
        return; 
    }
    new_block->record.id = id; 
    strcpy(new_block->record.name, name); 
    new_block->next = NULL; 

    if (file->head == NULL) {
        file->head = new_block; 
    } else {
        Block *current = file->head;
        while (current->next != NULL) {
            current = current->next; 
        }
        current->next = new_block; 
    }
}
void free_chained_file(ChainedFile *file) {
    Block *current = file->head; 
    while (current != NULL) {
        Block *temp = current; 
        current = current->next; 
        free(temp); 
    }
    file->head = NULL; 
}
void display_chained_file(const ChainedFile *file) {
    if (file->head == NULL) {
        printf("The chained file is empty.\n");
        return;
    }
    Block *current = file->head; 
    while (current != NULL) {
        printf("Record: ID=%d, Name=%s\n", current->record.id, current->record.name);
        current = current->next; 
    }
}
void compact_chained_file(ChainedFile *file) {
    Block *current = file->head; 
    Block *new_head = NULL; 
    Block *last_block = NULL; 

    while (current != NULL) {
        Block *new_block = (Block *)malloc(sizeof(Block)); 
        if (new_block == NULL) {
            printf("Error: Insufficient memory to allocate a new block during compaction.\n");
            return; 
        }
        new_block->record = current->record; 
        new_block->next = NULL; 

        if (new_head == NULL) {
            new_head = new_block; 
        } else {
            last_block->next = new_block; 
        }
        last_block = new_block; 
        current = current->next; 
    }
    free_chained_file(file); 
    file->head = new_head; 
}
void defragment_chained_file(ChainedFile *file) {
    Block *current = file->head; 
    Block *new_head = NULL; 
    Block *last_block = NULL; 
    while (current != NULL) {
        if (current->record.id != 0) { 
            Block *new_block = (Block *)malloc(sizeof(Block)); 
            if (new_block == NULL) {
                printf("Error: Insufficient memory to allocate a new block during defragmentation.\n");
                return; 
            }
            new_block->record = current->record; 
            new_block->next = NULL; 

            if (new_head == NULL) {
                new_head = new_block; 
            } else {
                last_block->next = new_block; 
            }
            last_block = new_block; 
        }
        current = current->next; 
    }
    
    free_chained_file(file); 
    file->head = new_head; 
}

// Function declarations
void initialize_contiguous_file(ContiguousFile *file);
void initialize_chained_file(ChainedFile *file);
int add_record_contiguous(ContiguousFile *file, int id, const char *name);
void free_contiguous_file(ContiguousFile *file);
void display_contiguous_file(const ContiguousFile *file);
void defragment_contiguous_file(ContiguousFile *file);
void add_record_chained(ChainedFile *file, int id, const char *name);
void free_chained_file(ChainedFile *file);
void display_chained_file(const ChainedFile *file);
void defragment_chained_file(ChainedFile *file);
void display_memory_state(ContiguousFile *contiguous_file, ChainedFile *chained_file);













