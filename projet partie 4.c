
typedef struct
{
    int id;
    char data[BLOCK_SIZE - sizeof(int)];
    int is_deleted; // 0 = active, 1 = deleted
} Record;

// Contiguous file structure
typedef struct
{
    Record records[MAX_RECORDS];
    int count;
} ContiguousFile;

// Chained file structure
typedef struct Node
{
    Record record;
    struct Node *next;
} Node;

typedef struct
{
    Node *head;
} ChainedFile;

// Initialize contiguous file
void init_contiguous_file(ContiguousFile *file)
{
    file->count = 0;
}

// Initialize chained file
void init_chained_file(ChainedFile *file)
{
    file->head = NULL;
}

// Logical delete in contiguous file
void logical_delete_contiguous(ContiguousFile *file, int id)
{
    for (int i = 0; i < file->count; i++)
    {
        if (file->records[i].id == id)
        {
            file->records[i].is_deleted = 1; // Mark as deleted
            printf("Record %d marked as deleted (logical).\n", id);
            return;
        }
    }
    printf("Record %d not found.\n", id);
}

// Physical delete in contiguous file
void physical_delete_contiguous(ContiguousFile *file, int id)
{
    for (int i = 0; i < file->count; i++)
    {
        if (file->records[i].id == id)
        {
            // Move subsequent records
            for (int j = i; j < file->count - 1; j++)
            {
                file->records[j] = file->records[j + 1];
            }
            file->count--; // Reduce the number of records
            printf("Record %d deleted (physical).\n", id);
            return;
        }
    }
    printf("Record %d not found.\n", id);
}

// Logical delete in chained file
void logical_delete_chained(ChainedFile *file, int id)
{
    Node *current = file->head;
    while (current != NULL)
    {
        if (current->record.id == id)
        {
            current->record.is_deleted = 1; // Mark as deleted
            printf("Record %d marked as deleted (logical).\n", id);
            return;
        }
        current = current->next;
    }
    printf("Record %d not found.\n", id);
}

// Physical delete in chained file
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
                // Deleting the first node
                file->head = current->next;
            }
            else
            {
                previous->next = current->next; // Update pointer
            }
            free(current); // Free the block
            printf("Record %d deleted (physical).\n", id);
            return;
        }
        previous = current;
        current = current->next;
    }
    printf("Record %d not found.\n", id);
}

// Function to add a record to the contiguous file
void add_record_contiguous(ContiguousFile *file, int id, const char *data)
{
    if (file->count < MAX_RECORDS)
    {
        file->records[file->count].id = id;
        strncpy(file->records[file->count].data, data, BLOCK_SIZE - sizeof(int));
        file->records[file->count].is_deleted = 0; // Mark as active
        file->count++;
    }
    else
    {
        printf("Contiguous file is full.\n");
    }
}

// Function to add a record to the chained file
void add_record_chained(ChainedFile *file, int id, const char *data)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->record.id = id;
    strncpy(new_node->record.data, data, BLOCK_SIZE - sizeof(int));
    new_node->record.is_deleted = 0; // Mark as active
    new_node->next = file->head;     // Insert at the beginning
    file->head = new_node;
}

// Example usage
int main()
{
    ContiguousFile contFile;
    ChainedFile chainFile;

    init_contiguous_file(&contFile);
    init_chained_file(&chainFile);

    // Adding records