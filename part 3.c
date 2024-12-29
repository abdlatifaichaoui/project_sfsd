#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 10
#define MAX_SIZE 10


typedef struct {
    int id;
    char name[50];
} Record;

// Contiguous organization
typedef struct {
    Record records[MAX_SIZE];
    int current_size;
} ContiguousFile;

// Chained organization
typedef struct Node {
    Record record;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
} ChainedFile;

// Secondary memory
typedef struct {
    ContiguousFile contiguous_files[MAX_FILES];
    ChainedFile chained_files[MAX_FILES];
    int file_count;
} SecondaryMemory;

// Functions for contiguous organization
void initialize_contiguous_file(ContiguousFile *file) {
    file->current_size = 0;
}

int add_record_contiguous(ContiguousFile *file, int id, const char *name) {
    if (file->current_size >= MAX_SIZE) {
        printf("Error: Insufficient space in the contiguous file.\n");
        return 0;
    }
    file->records[file->current_size].id = id;
    strcpy(file->records[file->current_size].name, name);
    file->current_size++;
    return 1;
}

void display_contiguous_file(const ContiguousFile *file) {
    if (file->current_size == 0) {
        printf("The contiguous file is empty.\n");
    } else {
        for (int i = 0; i < file->current_size; i++) {
            printf("Record %d: ID=%d, Name=%s\n", i + 1, file->records[i].id, file->records[i].name);
        }
    }
}

// Functions for chained organization
void initialize_chained_file(ChainedFile *file) {
    file->head = NULL;
}

void add_record_chained(ChainedFile *file, int id, const char *name) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        printf("Error: Insufficient memory.\n");
        return;
    }
    new_node->record.id = id;
    strcpy(new_node->record.name, name);
    new_node->next = NULL;

    if (file->head == NULL) {
        file->head = new_node;
    } else {
        Node *current = file->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

void display_chained_file(const ChainedFile *file) {
    if (file->head == NULL) {
        printf("The chained file is empty.\n");
    } else {
        Node *current = file->head;
        while (current != NULL) {
            printf("Record: ID=%d, Name=%s\n", current->record.id, current->record.name);
            current = current->next;
        }
    }
}

// File creation
void create_file(SecondaryMemory *memory) {
    if (memory->file_count >= MAX_FILES) {
        printf("Error: Maximum number of files reached.\n");
        return;
    }

    char type[10];
    printf("Organization type (contiguous/chained): ");
    scanf("%s", type);

    if (strcmp(type, "contiguous") == 0) {
        initialize_contiguous_file(&memory->contiguous_files[memory->file_count]);
        printf("Contiguous file created successfully.\n");
    } else if (strcmp(type, "chained") == 0) {
        initialize_chained_file(&memory->chained_files[memory->file_count]);
        printf("Chained file created successfully.\n");
    } else {
        printf("Invalid organization type.\n");
        return;
    }

    memory->file_count++;
}

// Record insertion
void insert_record(SecondaryMemory *memory, int file_index, const char *type, int id, const char *name) {
    if (file_index >= memory->file_count) {
        printf("Error: Invalid file index.\n");
        return;
    }

    if (strcmp(type, "contiguous") == 0) {
        add_record_contiguous(&memory->contiguous_files[file_index], id, name);
    } else if (strcmp(type, "chained") == 0) {
        add_record_chained(&memory->chained_files[file_index], id, name);
    } else {
        printf("Invalid organization type.\n");
    }
}

// Main function
int main() {
    SecondaryMemory memory = {0};

    int choice;
    do {
        printf("\n--- Menu ---\n");
        printf("1. Create a file\n");
        printf("2. Insert a record\n");
        printf("3. Display files\n");
        printf("4. Exit\n");
        printf("Your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                create_file(&memory);
                break;
            case 2: {
                int file_index, id;
                char type[10], name[50];
                printf("File index: ");
                scanf("%d", &file_index);
                printf("Organization type (contiguous/chained): ");
                scanf("%s", type);
                printf("ID: ");
                scanf("%d", &id);
                printf("Name: ");
                scanf("%s", name);
                insert_record(&memory, file_index, type, id, name);
                break;
            }
            case 3: {
                for (int i = 0; i < memory.file_count; i++) {
                    printf("\nFile %d:\n", i);
                    printf("Contiguous organization:\n");
                    display_contiguous_file(&memory.contiguous_files[i]);
                    printf("Chained organization:\n");
                    display_chained_file(&memory.chained_files[i]);
                }
                break;
            }
            case 4:
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 4);

    return 0;
}
