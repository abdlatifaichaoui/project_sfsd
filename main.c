#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#define NUM_BLOCKS 10  // Total num of blocks
#define BLOCK_SIZE 512 // block size 
#define MAX_FILES 10
#define MAX_SIZE 10
#define RED "\033[0;31m"
#define RESET "\033[0m"
#define GREEN "\033[0;32m"
#define RECORD_SIZE 64
#define MAX_FILENAME_LENGTH 15 
#define BLOCKS_PER_ROW 6 // for display

typedef struct
{
    int is_occupied; // 0 = Free, 1 = Occupied
    char data[BLOCK_SIZE];
    int record_count;  // number of records in the block
} Block;

typedef struct
{
    int id;
    int is_deleted;
} Record;
typedef struct Node
{
    Record record;
    struct Node *next;
} Node;
typedef struct
{
    Record records[MAX_SIZE];
    int max_size;
    int current_size;
    int sorted;
    int is_sorted;
} ContiguousFile;
typedef struct
{
    Node *head;

} ChainedFile;

typedef struct
{
    char name[15];
    int size;
    int first_block;
    int total_blocks;
    int is_sorted;
    int type; // 1 = Chained, 0 = Contiguous
} file;

Block virtual_disk[NUM_BLOCKS]; // Virtual disk with blocks
file files[MAX_FILES];
ContiguousFile contiguous_files[MAX_FILES];
ChainedFile chained_files[MAX_FILES];

typedef struct
{
    ContiguousFile contiguous_files[MAX_FILES];
    ChainedFile chained_files[MAX_FILES];
    int file_count;
} SecondaryMemory;
//////////////////////////////// intialization of secondary memory /////////////////////////////////////
void initialize_contiguous_file(ContiguousFile *file)
{
    file->current_size = 0;
    file->sorted = 1;
}
void initialize_chained_file(ChainedFile *file)
{
    file->head = NULL;
}
void initialize_allocation_table()
{
    for (int i = 0; i < NUM_BLOCKS; i++)
    {
        virtual_disk[i].is_occupied = 0;    // All blocks are free at initialization
        virtual_disk[i].record_count = 0;  // No records initially
        memset(virtual_disk[i].data, 0, BLOCK_SIZE); // Clear block data
    }

    for (int i = 0; i < MAX_FILES; i++)
    {
        files[i].name[0] = '\0';
        initialize_contiguous_file(&contiguous_files[i]);
        initialize_chained_file(&chained_files[i]);
    }
}
/////////////////////////////// handelig the blocks /////////////////////////////////////////////////
int find_free_block()
{
    for (int i = 0; i < NUM_BLOCKS; i++)
    {
        if (virtual_disk[i].is_occupied == 0)
        {
            return i; // Return the index of the first free block
        }
    }
    return -1; // No free block found
}
void allocate_block(int block_index)
{
    if (block_index >= 0 && block_index < NUM_BLOCKS)
    {
        virtual_disk[block_index].is_occupied = 1;
    }
}
void free_block(int block_index)
{
    if (block_index >= 0 && block_index < NUM_BLOCKS)
    {
        virtual_disk[block_index].is_occupied = 0;
        memset(virtual_disk[block_index].data, 0, BLOCK_SIZE); // Clear block data
    }
}
void compact_memory()
{
    int write_index = 0;

    for (int read_index = 0; read_index < NUM_BLOCKS; read_index++)
    {
        if (virtual_disk[read_index].is_occupied)
        {
            if (write_index != read_index)
            {
                // Move block data
                memcpy(&virtual_disk[write_index], &virtual_disk[read_index], sizeof(Block));
                virtual_disk[read_index].is_occupied = 0; // Mark old location as free
                memset(virtual_disk[read_index].data, 0, BLOCK_SIZE);
            }
            write_index++;
        }
    }
}
///////////////////////////////  handeling files  /////////////////////////////////////////////////

void create_file(const char *file_name, int file_size, int type, int is_sorted)
{

    if (file_size > NUM_BLOCKS)
    {
        printf("Error: File size exceeds available blocks.\n");
        return;
    }

    file new_file;
    strncpy(new_file.name, file_name, MAX_FILENAME_LENGTH);
    new_file.size = file_size;
    new_file.total_blocks = 0;
    new_file.first_block = -1;
    new_file.is_sorted = is_sorted;
    if (type == 0)
    {
        int start_block = -1;
        int contiguous_count = 0;

        // Find a set of contiguous free blocks
        for (int i = 0; i < NUM_BLOCKS; i++)
        {
            if (!virtual_disk[i].is_occupied)
            {
                if (start_block == -1)
                {
                    start_block = i; // Start of a new contiguous segment
                }

                contiguous_count++;

                if (contiguous_count == file_size)
                {
                    // Allocate these blocks
                    for (int j = start_block; j < start_block + file_size; j++)
                    {
                        allocate_block(j);
                    }
                    new_file.first_block = start_block;
                    new_file.total_blocks = file_size;
                    new_file.type = 0;
                    initialize_contiguous_file(&contiguous_files[start_block]);
                    printf("Contiguous file '%s' created successfully.\n", file_name);
                    break;
                }
            }
            else
            {
                if (start_block != -1)
                {
                    printf("Debug: Contiguous block sequence broken at index %d.\n", i);
                }
                start_block = -1;     // Reset the start block
                contiguous_count = 0; // Reset the count
            }
        }

        // Check if contiguous blocks were successfully allocated
        if (new_file.first_block == -1)
        {
            printf("Error: Could not allocate %d contiguous blocks for file '%s'.\n", file_size, file_name);
        }
    }

    else if (type == 1) // Chained file
{
    int allocated_blocks = 0;

    for (int i = 0; i < NUM_BLOCKS && allocated_blocks < file_size; i++)
    {
        if (!virtual_disk[i].is_occupied)
        {
            if (new_file.first_block == -1)
            {
                new_file.first_block = i; // Set the first block
            }
            allocate_block(i);

            virtual_disk[i].is_occupied = 1; // Mark block as occupied
            virtual_disk[i].record_count = 0; // Initialize record count to 0
            allocated_blocks++;
        }
    }

    new_file.total_blocks = allocated_blocks;
    new_file.type = 1;

    if (allocated_blocks < file_size)
    {
        printf("Error: Not enough blocks available for chained file.\n");
        return;
    }

    initialize_chained_file(&chained_files[new_file.first_block]);
    printf("Chained file '%s' created successfully.\n", file_name);
}

    else
    {
        printf("Invalid organization type.\n");
        return;
    }

    // Store the file in the files array
    for (int i = 0; i < MAX_FILES; i++)
    {
        if (files[i].name[0] == '\0')
        { // Find an empty slot
            files[i] = new_file;
            return;
        }
    }

    printf("Error: Maximum number of files reached.\n");
}
int rename_file(char *old_name, char *new_name)
{
    int i;
    for (i = 0; i < NUM_BLOCKS; i++)
    {
        if (files[i].name[0] != '\0' && strcmp(files[i].name, old_name) == 0)
        {

            strncpy(files[i].name, new_name, MAX_FILENAME_LENGTH);
            printf("File '%s' renamed to '%s'.\n", old_name, new_name);

            return 0;
        }
    }
    printf("Error: File '%s' not found.\n", old_name);
    return -1;
}
int delete_file(const char *filename)
{
    int i;
    for (i = 0; i < NUM_BLOCKS; i++)
    {
        if (files[i].name[0] != '\0' && strcmp(files[i].name, filename) == 0)
        {
            int block = files[i].first_block;
            int j;
            for (j = 0; j < files[i].total_blocks; j++)
            {
                virtual_disk[block].is_occupied = 0; // Mark blocks as free
                block++;
            }
            printf("File '%s' deleted.\n", filename);
            files[i].name[0] = '\0'; // Clear the file metadata
            return 0;
        }
    }
    printf("Error: File '%s' not found.\n", filename);
    return -1;
}

////////////////////////////// search funtions /////////////////////////////////////
int sequential_search(ContiguousFile *file, int id, int *index)
{
    for (int i = 0; i < file->current_size; i++)
    {
        if (file->records[i].id == id && file->records[i].is_deleted == 0)
        {
            *index = i;
            return 1;
        }
    }
    return 0;
}
int binary_search(ContiguousFile *file, int id, int *index)
{
    int left = 0, right = file->current_size - 1;
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        if (file->records[mid].id == id && file->records[mid].is_deleted == 0)
        {
            *index = mid;
            return 1;
        }
        if (file->records[mid].id < id)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    return 0;
}
int search_contiguous(ContiguousFile *file, int id, int *index)
{
    if (file->is_sorted)
    {
        return binary_search(file, id, index);
    }
    else
    {
        return sequential_search(file, id, index);
    }
}
Node *search_chained(ChainedFile *file, int id)
{
    Node *current = file->head;
    while (current != NULL)
    {
        if (current->record.id == id && current->record.is_deleted == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}
/////////////////////////////// record management  ///////////////////////////////////////////////

//**************************** inserting a record ************************************//
int add_record_chained(ChainedFile *file, int id, int is_sorted)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node)
    {
        printf("Error: Insufficient memory.\n");
        return 0;
    }

    new_node->record.id = id;
    new_node->record.is_deleted = 0;
    new_node->next = NULL;

    if (is_sorted)
    {
        Node **current = &file->head;
        while (*current && (*current)->record.id < id)
        {
            current = &((*current)->next);
        }
        new_node->next = *current;
        *current = new_node;
    }
    else
    {
        if (!file->head)
        {
            file->head = new_node;
        }
        else
        {
            Node *temp = file->head;
            while (temp->next)
            {
                temp = temp->next;
            }
            temp->next = new_node;
        }
    }

    // Increment the record count for the block
    int block_index = file - chained_files;
    virtual_disk[block_index].record_count++;
    return 1;
}
int add_record_contiguous(ContiguousFile *file, int id, int is_sorted)
{
    if (file->current_size >= MAX_SIZE)
    {
        printf("Error: Insufficient space in the contiguous file.\n");
        return 0;
    }

    int block_index = file - contiguous_files; // Find the block index
    int records_per_block = BLOCK_SIZE / RECORD_SIZE;

    if (is_sorted)
    {
        // Insert in sorted order
        int pos = file->current_size;
        while (pos > 0 && file->records[pos - 1].id > id)
        {
            file->records[pos] = file->records[pos - 1];
            pos--;
        }
        file->records[pos].id = id;
        file->records[pos].is_deleted = 0;
    }
    else
    {
        // Append at the end
        file->records[file->current_size].id = id;
        file->records[file->current_size].is_deleted = 0;
    }

    // Update the record count in the appropriate block
    int current_block = block_index + (file->current_size / records_per_block);
    virtual_disk[current_block].record_count++;

    file->current_size++;
    return 1;
}
int insert_record(const char *filename, int id)
{
    int i;
    for (i = 0; i < NUM_BLOCKS; i++)
    {
        if (files[i].name[0] != '\0' && strcmp(files[i].name, filename) == 0)
        {
            if (files[i].type == 0)
            {
                add_record_contiguous(&contiguous_files[files[i].first_block], id, files[i].is_sorted);
                return 1;
            }
            else if (files[i].type == 1)
            {
                add_record_chained(&chained_files[files[i].first_block], id, files[i].is_sorted);
                return 1;
            }
        }
    }
    printf("Error: File '%s' not found.\n", filename);
    return 0;
}

//****************************** deleting a record   **********************************//
void logical_delete_contiguous(ContiguousFile *file, int id)
{
    int index;
    if (search_contiguous(file, id, &index))
    {
        file->records[index].is_deleted = 1;
        printf("Record %d marked as deleted (logical).\n", id);
    }
    else
    {
        printf("Record %d not found.\n", id);
    }
}
void logical_delete_chained(ChainedFile *file, int id)
{
    Node *node = search_chained(file, id);
    if (node != NULL)
    {
        node->record.is_deleted = 1;
        printf("Record %d marked as deleted (logical).\n", id);
    }
    else
    {
        printf("Record %d not found.\n", id);
    }
}
void physical_delete_contiguous(ContiguousFile *file, int id)
{
    int index;
    if (search_contiguous(file, id, &index))
    {
        for (int i = index; i < file->current_size - 1; i++)
        {
            file->records[i] = file->records[i + 1];
        }
        file->current_size--;
        printf("Record %d deleted (physical).\n", id);
    }
    else
    {
        printf("Record %d not found.\n", id);
    }
}
void physical_delete_chained(ChainedFile *file, int id)
{
    Node *current = file->head;
    Node *previous = NULL;

    while (current != NULL)
    {
        if (current->record.id == id)
        {
            if (previous == NULL)
            {
                file->head = current->next;
            }
            else
            {
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
void logical_delete(const char *filename, int id) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].name[0] != '\0' && strcmp(files[i].name, filename) == 0) {
            if (files[i].type == 0) {
                logical_delete_contiguous(&contiguous_files[files[i].first_block], id);
            } else if (files[i].type == 1) {
                logical_delete_chained(&chained_files[files[i].first_block], id);
            }
            return;
        }
    }
    printf("Error: File '%s' not found.\n", filename);
}
void physical_delete(const char *filename, int id) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].name[0] != '\0' && strcmp(files[i].name, filename) == 0) {
            if (files[i].type == 0) {
                physical_delete_contiguous(&contiguous_files[files[i].first_block], id);
            } else if (files[i].type == 1) {
                physical_delete_chained(&chained_files[files[i].first_block], id);
            }
            return;
        }
    }
    printf("Error: File '%s' not found.\n", filename);
}

//////////////////////////////// displaying functions /////////////////////////////////////
void display_contiguous_file(const ContiguousFile *file)
{
    if (file->current_size == 0)
    {
        printf("The contiguous file is empty.\n");
    }
    else
    {
        printf("Sorted: %s\n", file->sorted ? "Yes" : "No");
        for (int i = 0; i < file->current_size; i++)
        {
            printf("Record %d: ID=%d", i + 1, file->records[i].id);
            if (file->records[i].is_deleted)
            {
                printf(" (deleted)");
            }
            printf("\n");
            
        }
    }
}
void display_chained_file(const ChainedFile *file)
{
    if (file->head == NULL)
    {
        printf("The chained file is empty.\n");
    }
    else
    {

        Node *current = file->head;
        while (current != NULL)
        {
                printf("Record: ID=%d", current->record.id);
            if (current->record.is_deleted)
            {
                printf(" (deleted)");
            }
            printf("\n");
            current = current->next;
        }
    }
}
void display_records(const char *filename) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].name[0] != '\0' && strcmp(files[i].name, filename) == 0) {
            if (files[i].type == 0) {
                display_contiguous_file(&contiguous_files[files[i].first_block]);
            } else if (files[i].type == 1) {
                display_chained_file(&chained_files[files[i].first_block]);
            }
            return;
        }
    }
    printf("Error: File '%s' not found.\n", filename);
}
void display_allocation_table()
{
    printf("\n--- Memory Allocation Table ---\n");
    printf("Legend: " GREEN "[Green = Free]" RESET ", " RED "[Red = Occupied]" RESET "\n\n");

    for (int i = 0; i < NUM_BLOCKS; i++)
    {
        if (!virtual_disk[i].is_occupied)
        {
            printf(GREEN "[%02d: Free]" RESET " ", i);
        }
        else
        {
            char *file_name = NULL;

            for (int j = 0; j < MAX_FILES; j++)
            {
                if (files[j].name[0] != '\0' &&
                    i >= files[j].first_block &&
                    i < files[j].first_block + files[j].total_blocks)
                {
                    file_name = files[j].name;
                    break;
                }
            }

            if (file_name)
            {
                printf(RED "[%02d: %s, %dR]" RESET " ", i, file_name, virtual_disk[i].record_count);
            }
            else
            {
                printf(RED "[%02d: Unknown]" RESET " ", i);
            }
        }

        if ((i + 1) % BLOCKS_PER_ROW == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
}





void menu()
{
    int choice;
    char file_name[MAX_FILENAME_LENGTH];
    char new_name[MAX_FILENAME_LENGTH];
    int num_records, file_type, is_sorted, insert_record_id, id;
    SecondaryMemory memory;
    initialize_allocation_table();

    while (1)
    {
        printf("\n--- Menu ---\n");
        printf("1. Create File\n");
        printf("2. Delete File\n");
        printf("3. Rename File\n");
        printf("4. Display Allocation Table\n");
        printf("5. Insert Record\n");
        printf("6. Display Records in File\n");
        printf("7. Logical Delete Record\n");
        printf("8. Physical Delete Record\n");
        printf("9. Compact Memory\n");
        printf("10. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter file name: ");
            scanf("%s", file_name);
            printf("Enter number of records: ");
            scanf("%d", &num_records);
            printf("Enter file type (contiguous = 0 / chained = 1): ");
            scanf("%d", &file_type);
            printf("Is the file sorted? (0 = No, 1 = Yes): ");
            scanf("%d", &is_sorted);
            int num_blocks = (num_records * RECORD_SIZE + BLOCK_SIZE - 1) / BLOCK_SIZE;
            create_file(file_name, num_blocks, file_type, is_sorted);
            if (file_type == 0)
            {
                contiguous_files[find_free_block()].is_sorted = is_sorted;
            }
            break;
        case 2:
            printf("Enter file name to delete: ");
            scanf("%s", file_name);
            delete_file(file_name);
            break;
        case 3:
            printf("Enter current file name: ");
            scanf("%s", file_name);
            printf("Enter new file name: ");
            scanf("%s", new_name);
            rename_file(file_name, new_name);
            break;
        case 4:
            display_allocation_table();
            break;
        case 5:
            printf("Enter file name: ");
            scanf("%s", file_name);
            printf("enter record id :");
            scanf("%d", &insert_record_id);
            insert_record(file_name, insert_record_id);
            break;
       case 6: 
    printf("Enter file name: ");
    scanf("%s", file_name);
    display_records(file_name);
    break;
        case 7: 
    printf("Enter file name: ");
    scanf("%s", file_name);
    printf("Enter record ID to logically delete: ");
    scanf("%d", &id);
    logical_delete(file_name, id);
  
    break;
case 8: 
    printf("Enter file name: ");
    scanf("%s", file_name);
    printf("Enter record ID to physically delete: ");
    scanf("%d", &id);
    physical_delete(file_name, id);
    break;

        case 9:
            compact_memory();
            printf("Memory compacted successfully.\n");
            break;
        case 10:
            printf("Exiting program.\n");
            return;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
}

int main()
{
    menu();
    return 0;
}
