#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 128
#define MAX_RECORDS 100
#define RECORD_SIZE 20

typedef struct {
    int id;
    char data[RECORD_SIZE];
    int is_deleted; // 0 = active, 1 = deleted
} Record;

// Contiguous file structure
typedef struct {
    Record records[MAX_RECORDS];
    int count;
    int is_sorted; // Boolean: 1 if sorted, 0 if not
} ContiguousFile;

// Chained file structure
typedef struct Node {
    Record record;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
} ChainedFile;

// Initialize contiguous file
void init_contiguous_file(ContiguousFile *file, int is_sorted) {
    file->count = 0;
    file->is_sorted = is_sorted;
}

// Initialize chained file
void init_chained_file(ChainedFile *file) {
    file->head = NULL;
}

// Perform sequential search for a record in unsorted contiguous file
int sequential_search(ContiguousFile *file, int id, int *index) {
    for (int i = 0; i < file->count; i++) {
        if (file->records[i].id == id && file->records[i].is_deleted == 0) {
            *index = i;
            return 1;
        }
    }
    return 0;
}

// Perform binary search for a record in sorted contiguous file
int binary_search(ContiguousFile *file, int id, int *index) {
    int left = 0, right = file->count - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (file->records[mid].id == id && file->records[mid].is_deleted == 0) {
            *index = mid;
            return 1;
        }
        if (file->records[mid].id < id) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return 0;
}

// Search for a record in a contiguous file
int search_contiguous(ContiguousFile *file, int id, int *index) {
    if (file->is_sorted) {
        return binary_search(file, id, index);
    } else {
        return sequential_search(file, id, index);
    }
}

// Search for a record in a chained file
Node* search_chained(ChainedFile *file, int id) {
    Node *current = file->head;
    while (current != NULL) {
        if (current->record.id == id && current->record.is_deleted == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Logical delete in contiguous file
void logical_delete_contiguous(ContiguousFile *file, int id) {
    int index;
    if (search_contiguous(file, id, &index)) {
        file->records[index].is_deleted = 1;
        printf("Record %d marked as deleted (logical).\n", id);
    } else {
        printf("Record %d not found.\n", id);
    }
}

// Logical delete in chained file
void logical_delete_chained(ChainedFile *file, int id) {
    Node *node = search_chained(file, id);
    if (node != NULL) {
        node->record.is_deleted = 1;
        printf("Record %d marked as deleted (logical).\n", id);
    } else {
        printf("Record %d not found.\n", id);
    }
}

// Physical delete in contiguous file
void physical_delete_contiguous(ContiguousFile *file, int id) {
    int index;
    if (search_contiguous(file, id, &index)) {
        for (int i = index; i < file->count - 1; i++) {
            file->records[i] = file->records[i + 1];
        }
        file->count--;
        printf("Record %d deleted (physical).\n", id);
    } else {
        printf("Record %d not found.\n", id);
    }
}

// Physical delete in chained file
void physical_delete_chained(ChainedFile *file, int id) {
    Node *current = file->head;
    Node *previous = NULL;

    while (current != NULL) {
        if (current->record.id == id) {
            if (previous == NULL) {
                file->head = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            printf("Record %d deleted (physical).\n", id);
            return;
        }
        previous = current;
        current = current->next;
    }
    printf("Record %d not found.\n", id);
}
/////////////////********************************just for testing**************////////////////////////////////
// Add a record to the contiguous file
void add_record_contiguous(ContiguousFile *file, int id, const char *data) {
    if (file->count < MAX_RECORDS) {
        file->records[file->count].id = id;
        strncpy(file->records[file->count].data, data, RECORD_SIZE - 1);
        file->records[file->count].data[RECORD_SIZE - 1] = '\0';
        file->records[file->count].is_deleted = 0;
        file->count++;

        // Ensure sorted order if the file is marked as sorted
        if (file->is_sorted) {
            for (int i = file->count - 1; i > 0 && file->records[i].id < file->records[i - 1].id; i--) {
                Record temp = file->records[i];
                file->records[i] = file->records[i - 1];
                file->records[i - 1] = temp;
            }
        }
    } else {
        printf("Contiguous file is full.\n");
    }
}

// Add a record to the chained file
void add_record_chained(ChainedFile *file, int id, const char *data) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->record.id = id;
    strncpy(new_node->record.data, data, RECORD_SIZE - 1);
    new_node->record.data[RECORD_SIZE - 1] = '\0';
    new_node->record.is_deleted = 0;
    new_node->next = file->head;
    file->head = new_node;
}

void interactive_menu() {
    ContiguousFile contFile;
    ChainedFile chainFile;
    init_contiguous_file(&contFile, 1); // Initialize sorted contiguous file
    init_chained_file(&chainFile);

    int choice;
    do {
        printf("\nMenu:\n");
        printf("1. Add record to Contiguous File\n");
        printf("2. Add record to Chained File\n");
        printf("3. Logical delete in Contiguous File\n");
        printf("4. Logical delete in Chained File\n");
        printf("5. Physical delete in Contiguous File\n");
        printf("6. Physical delete in Chained File\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        int id;
        char data[RECORD_SIZE];

        switch (choice) {
            case 1:
                printf("Enter ID and Data to add to Contiguous File: ");
                scanf("%d %s", &id, data);
                add_record_contiguous(&contFile, id, data);
                break;

            case 2:
                printf("Enter ID and Data to add to Chained File: ");
                scanf("%d %s", &id, data);
                add_record_chained(&chainFile, id, data);
                break;

            case 3:
                printf("Enter ID to logically delete from Contiguous File: ");
                scanf("%d", &id);
                logical_delete_contiguous(&contFile, id);
                break;

            case 4:
                printf("Enter ID to logically delete from Chained File: ");
                scanf("%d", &id);
                logical_delete_chained(&chainFile, id);
                break;

            case 5:
                printf("Enter ID to physically delete from Contiguous File: ");
                scanf("%d", &id);
                physical_delete_contiguous(&contFile, id);
                break;

            case 6:
                printf("Enter ID to physically delete from Chained File: ");
                scanf("%d", &id);
                physical_delete_chained(&chainFile, id);
                break;

            case 7:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 7);
}

int main() {
    interactive_menu();
    return 0;
}
