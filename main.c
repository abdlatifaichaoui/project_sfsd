#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#define NUM_BLOCKS 100  // Total num of blocks
#define BLOCK_SIZE 640 // block size 
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

typedef struct MetaData {
    char fileName[50];
    int fileSize;
    int usedBlocks;
    int recordCount;
    int firstBlockAddress;
} MetaData;

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
{int i;
    for ( i = 0; i < NUM_BLOCKS; i++)
    {
        virtual_disk[i].is_occupied = 0;    // All blocks are free at initialization
        virtual_disk[i].record_count = 0;  // No records initially
        memset(virtual_disk[i].data, 0, BLOCK_SIZE); // Clear block data
    }

    for ( i = 0; i < MAX_FILES; i++)
    {
	remove(files[i].name);
        files[i].name[0] = '\0';
        initialize_contiguous_file(&contiguous_files[i]);
        initialize_chained_file(&chained_files[i]);
    }
}
/////////////////////////////// handelig the blocks /////////////////////////////////////////////////
int find_free_block()
{int i;
    for ( i = 0; i < NUM_BLOCKS; i++)
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
int read_index;
    for ( read_index = 0; read_index < NUM_BLOCKS; read_index++)
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

/////////////////////////////// MetaDataFile ////////////////////////////////////////////////
// Read metadata
void readMetaData(FILE *f,MetaData *meta) {

    rewind(f);
    fread(meta, sizeof(MetaData), 1, f);
  
}

// Update metadata
void updateMetaData(FILE *f, MetaData *meta) {
    rewind(f);
    fwrite(meta, sizeof(MetaData), 1, f);
}

// Create metadata file
void createMetaData(FILE *f, const char *fileName, int fileSize, int blockCount, int firstBlockAddress) {
    MetaData meta;
    strcpy(meta.fileName, fileName);
    meta.fileSize = fileSize;
    meta.usedBlocks = blockCount;
    meta.recordCount = 0;
    meta.firstBlockAddress = firstBlockAddress;

    updateMetaData(f, &meta);
}

// Display metadata
void displayMetaData(const MetaData *meta) {
    printf("File name: %s\n", meta->fileName);
    printf("Total file size: %d bytes\n", meta->fileSize);
    printf("Number of used blocks: %d\n", meta->usedBlocks);
    printf("First block address: %d\n", meta->firstBlockAddress);
}

///////////////////////////////  handeling files  /////////////////////////////////////////////////

void create_file(const char *file_name, int file_size, int type, int is_sorted,int num_records)
{
 printf("%d",file_size);
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
        int i;
        for ( i = 0; i < NUM_BLOCKS; i++)
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
                    int j;
                    for ( j = start_block; j < start_block + file_size; j++)
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

           // create filemetadata 
            // Open a file in write mode
    FILE *f = fopen(file_name, "w");

    // Check if the file was created/opened successfully
    if (f == NULL) {
        printf("Error opening the file.\n");
        
    }
           
           createMetaData(f, file_name, 0, file_size,start_block);
            fclose(f);
        // Check if contiguous blocks were successfully allocated
        if (new_file.first_block == -1)
        {
            printf("Error: Could not allocate %d contiguous blocks for file '%s'.\n", file_size, file_name);
        }
    }

    else if (type == 1) // Chained file
{
    int allocated_blocks = 0;
int i;
    for ( i = 0; i < NUM_BLOCKS && allocated_blocks < file_size; i++)
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
     FILE *f = fopen(file_name, "w");

    // Check if the file was created/opened successfully
    if (f == NULL) {
        printf("Error opening the file.\n");
        
    }
           
           createMetaData(f, file_name, 0, allocated_blocks,i);
            fclose(f);
}

    else
    {
        printf("Invalid organization type.\n");
        return;
    }

    // Store the file in the files array 
    int i;
    for (i = 0; i < MAX_FILES; i++)
    {
        if (files[i].name[0] == '\0')
        { // Find an empty slot
            files[i] = new_file;
            return;
        }
    }

    printf("Error: Maximum number of files reached.\n");
}
int rename_file(char *old_name, char *new_name,MetaData meta)
{
    int i;
    for (i = 0; i < NUM_BLOCKS; i++)
    {
        if (files[i].name[0] != '\0' && strcmp(files[i].name, old_name) == 0)
        {

            strncpy(files[i].name, new_name, MAX_FILENAME_LENGTH);
            printf("File '%s' renamed to '%s'.\n", old_name, new_name);
            rename(old_name, new_name) ;
            FILE *f = fopen(new_name, "r+b");

    // Check if the file was created/opened successfully
    if (f == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }
            readMetaData(f,&meta);
           strcpy(meta.fileName, new_name);
            updateMetaData(f,&meta);
              fclose(f);

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
{int i;
    for ( i = 0; i < file->current_size; i++)
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
int insert_record(const char *filename, int id,MetaData meta)
{
    int i;
    for (i = 0; i < NUM_BLOCKS; i++)
    {
        if (files[i].name[0] != '\0' && strcmp(files[i].name, filename) == 0)
        {
            if (files[i].type == 0)
            {
                add_record_contiguous(&contiguous_files[files[i].first_block], id, files[i].is_sorted);
                 FILE *f1 = fopen(filename, "r+b");

    // Check if the file was created/opened successfully
    if (f1 == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }
            readMetaData(f1,&meta);
         
           meta.fileSize = meta.fileSize + (1* RECORD_SIZE);
          
            updateMetaData(f1, &meta);
              fclose(f1);
                return 1;
            }
            else if (files[i].type == 1)
            {
                add_record_chained(&chained_files[files[i].first_block], id, files[i].is_sorted);
                 FILE *f1 = fopen(filename, "r+b");

    // Check if the file was created/opened successfully
    if (f1 == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }
            readMetaData(f1,&meta);
         
           meta.fileSize = meta.fileSize + (1* RECORD_SIZE);
          
            updateMetaData(f1, &meta);
              fclose(f1);
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
          int block_index = file - contiguous_files; // Find the block index
    int records_per_block = BLOCK_SIZE / RECORD_SIZE;
     int current_block = block_index + (file->current_size / records_per_block);
    virtual_disk[current_block].record_count--;
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
         int block_index = file - chained_files;
    virtual_disk[block_index].record_count--;
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
    {int i;
        for ( i = index; i < file->current_size - 1; i++)
        {
            file->records[i] = file->records[i + 1];
        }
        file->current_size--;
          int block_index = file - contiguous_files; // Find the block index
    int records_per_block = BLOCK_SIZE / RECORD_SIZE;
     int current_block = block_index + (file->current_size / records_per_block);
    virtual_disk[current_block].record_count--;
       
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
             int block_index = file - chained_files;
    		virtual_disk[block_index].record_count--;
          
            printf("Record %d deleted (physical).\n", id);
            return;
        }
        previous = current;
        current = current->next;
    }
    printf("Record %d not found.\n", id);
}
void logical_delete(const char *filename, int id,MetaData meta) {
	int i;
    for ( i = 0; i < MAX_FILES; i++) {
        if (files[i].name[0] != '\0' && strcmp(files[i].name, filename) == 0) {
            if (files[i].type == 0) {
                logical_delete_contiguous(&contiguous_files[files[i].first_block], id);
            } else if (files[i].type == 1) {
                logical_delete_chained(&chained_files[files[i].first_block], id);
            }
             FILE *f3 = fopen(filename, "r+b");

    // Check if the file was created/opened successfully
    if (f3 == NULL) {
        printf("Error opening the file.\n");
        return ;
    }
            readMetaData(f3,&meta);
           meta.fileSize = meta.fileSize - (1* RECORD_SIZE);
            updateMetaData(f3,&meta);
              fclose(f3);
            return;
        }
    }
    printf("Error: File '%s' not found.\n", filename);
}
void physical_delete(const char *filename, int id,MetaData meta) {
	int i;
    for ( i = 0; i < MAX_FILES; i++) {
        if (files[i].name[0] != '\0' && strcmp(files[i].name, filename) == 0) {
            if (files[i].type == 0) {
                physical_delete_contiguous(&contiguous_files[files[i].first_block], id);
            } else if (files[i].type == 1) {
                physical_delete_chained(&chained_files[files[i].first_block], id);
            }
            FILE *f2 = fopen(filename, "r+b");

    // Check if the file was created/opened successfully
    if (f2 == NULL) {
        printf("Error opening the file.\n");
        return ;
    }
            readMetaData(f2,&meta);
           meta.fileSize = meta.fileSize - (1* RECORD_SIZE);
            updateMetaData(f2,&meta);
              fclose(f2);
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
        int i;
        for ( i = 0; i < file->current_size; i++)
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
	int i;
    for ( i = 0; i < MAX_FILES; i++) {
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
	system("cls");

    printf("\n\n--- Memory Visualization ---\n\n");
    printf("Legend: " GREEN "[Green = Free]" RESET ", " RED "[Red = Occupied]" RESET "\n\n");
int i;
    for ( i = 0; i < NUM_BLOCKS; i++)
    {
        if (!virtual_disk[i].is_occupied)
        {
            printf( GREEN "[%02d: Free]" RESET " ", i);
        }
        else
        {
            char *file_name = NULL;
			int j;
            for ( j = 0; j < MAX_FILES; j++)
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
    printf("\n");
}

int back(){
printf("\n");
	
    char backKey;
    printf("Press any key to go back to the main menu: ");
   

    #ifdef _WIN32
        _getch(); 
    	#ifdef _WIN32
        system("cls");
	#endif
		#endif
        return ;
   
}



void menu()
{
    int choice;
    char file_name[MAX_FILENAME_LENGTH];
    char new_name[MAX_FILENAME_LENGTH];
    int num_records, file_type, is_sorted, insert_record_id, id;
    SecondaryMemory memory;
    MetaData meta;
    initialize_allocation_table();

    while (1)
    {
        printf("\n  Menu: \n\n");
        printf("1. Create File\n");
        printf("2. Delete File\n");
        printf("3. Rename File\n");
        printf("4. Display Memory state\n");
        printf("5. Insert Record\n");
        printf("6. Display Records in File\n");
        printf("7. Logical Delete Record\n");
        printf("8. Physical Delete Record\n");
        printf("9. Compact Memory\n");
        printf("10. Display the metadata file \n");
        printf("11. Clear memory \n");
        printf("12. Exit program \n \n");
       
         do {
       printf("Enter your choice:  ");
        
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');  
            printf("Invalid input. Please enter a valid number between 1 and 12.\n");
        } else {
            
            if (getchar() != '\n') {
                // Clear the remaining input characters
                while (getchar() != '\n');  // Clear the input buffer
                printf("Invalid input. Please enter a number between 1 and 12.\n");
            } else if (choice < 1 || choice > 12) {
                printf("Invalid choice. Please enter a number between 1 and 12.\n");
            }
        }
    } while (choice < 1 || choice > 12);

        switch (choice)
        {
        case 1:
        	system("cls");
            printf("Enter file name: ");
            scanf("%s", file_name);
		int i;
         		int a = 0;
             for ( i = 0; i < MAX_FILES; i++){
             	if(strcmp(file_name,files[i].name) == 0){
             		a = 1;
				 }
			 }
			 if(a == 0){
           while (1) {  // Infinite loop to keep asking for valid input
        printf("Enter number of records: ");
        
        // Try to read an integer
        if (scanf("%d", &num_records) != 1) {
            // If the input is not an integer, clear the input buffer
            while (getchar() != '\n');  // Clear the input buffer
            
            // Print an error message
            printf("Invalid input. Please enter a valid number.\n");
        } else {
            // If the input is a valid integer, break the loop
            break;
        }
    }
          while (1) {
        printf("Enter file type (contiguous = 0 / chained = 1): ");
        if (scanf("%d", &file_type) != 1 || (file_type != 0 && file_type != 1)) {
            // If input is not valid or not 0 or 1, clear input buffer and show an error
            while (getchar() != '\n');  // Clear the input buffer
            printf("Invalid input. Please enter 0 for contiguous or 1 for chained.\n");
        } else {
            break;  // Valid input, exit the loop
        }
    }

    // Handle is_sorted input
    while (1) {
        printf("Is the file sorted? (0 = No, 1 = Yes): ");
        if (scanf("%d", &is_sorted) != 1 || (is_sorted != 0 && is_sorted != 1)) {
            // If input is not valid or not 0 or 1, clear input buffer and show an error
            while (getchar() != '\n');  // Clear the input buffer
            printf("Invalid input. Please enter 0 for No or 1 for Yes.\n");
        } else {
            break;  // Valid input, exit the loop
        }
    }
            int num_blocks = (num_records * RECORD_SIZE + BLOCK_SIZE - 1) / BLOCK_SIZE;
            printf("%d",num_blocks);
            create_file(file_name, num_blocks, file_type, is_sorted,num_records);
            if (file_type == 0)
            {
                contiguous_files[find_free_block()].is_sorted = is_sorted;
            }
	            back();}
		else {
            	printf("this file already exist");
            	back();
			}
            break;
        case 2:
        	system("cls");
            printf("Enter file name to delete: ");
            scanf("%s", file_name);
            delete_file(file_name);
            remove(file_name);
            back();
            break;
        case 3:
        	system("cls");
            printf("Enter current file name: ");
            scanf("%s", file_name);
            printf("Enter new file name: ");
            scanf("%s", new_name);
            rename_file(file_name, new_name,meta);
           
            back();
            break;
        case 4:
        	
            display_allocation_table();
            back();
            break;
        case 5:
        	system("cls");
            printf("Enter file name: ");
            scanf("%s", file_name);
            while (1) {  // Infinite loop to keep asking for valid input
        printf("enter record id :");
        
        // Try to read an integer
        if (scanf("%d", &insert_record_id) != 1) {
            // If the input is not an integer, clear the input buffer
            while (getchar() != '\n');  // Clear the input buffer
            
            // Print an error message
            printf("Invalid input. Please enter a valid number.\n");
        } else {
            // If the input is a valid integer, break the loop
            break;
        }
    }
            insert_record(file_name, insert_record_id,meta);
            
            back();
            break;
       case 6: 
       system("cls");
    printf("Enter file name: ");
    scanf("%s", file_name);
    display_records(file_name);
    back();
    break;
        case 7: 
        system("cls");
    printf("Enter file name: ");
    scanf("%s", file_name);
    while (1) {  // Infinite loop to keep asking for valid input
        printf("Enter record ID to logically delete: ");
        
        // Try to read an integer
        if (scanf("%d", &id) != 1) {
            // If the input is not an integer, clear the input buffer
            while (getchar() != '\n');  // Clear the input buffer
            
            // Print an error message
            printf("Invalid input. Please enter a valid number.\n");
        } else {
            // If the input is a valid integer, break the loop
            break;
        }
    }
    logical_delete(file_name, id,meta);
    
   
  back();
    break;
case 8: 
		system("cls");
    printf("Enter file name: ");
    scanf("%s", file_name);
    while (1) {  // Infinite loop to keep asking for valid input
         printf("Enter record ID to physically delete: ");
        // Try to read an integer
        if (scanf("%d", &id) != 1) {
            // If the input is not an integer, clear the input buffer
            while (getchar() != '\n');  // Clear the input buffer
            
            // Print an error message
            printf("Invalid input. Please enter a valid number.\n");
        } else {
            // If the input is a valid integer, break the loop
            break;
        }
    }
    physical_delete(file_name, id,meta);
   
    back();
    break;

        case 9:
        	system("cls");
        	printf("Compactage memory ......... \n");
            compact_memory();
         
            printf("Memory compacted successfully.\n ");
            back();
            break;
            
        case 10:
        	system("cls");
        	 printf("10. Display the metadata file \n");
        	  printf("Enter file name: ");
    			scanf("%s", file_name);
    			FILE *file = fopen(file_name, "r");

    // Check if the file was created/opened successfully
    if (file == NULL) {
        printf("Error opening the file.\n");
        return ;
    }
        readMetaData(file,&meta);
         fclose(file);
        displayMetaData(&meta);
        
            back();
		break;
		case 11:
		system("cls");
        printf("Clearing memory  ........ \n");
        printf("Memory cleared succesfully \n");
         initialize_allocation_table();
        back();
		break;    
        case 12:
        	
            printf("Exiting program .......\n");
            return;
        default:
            printf("Invalid choice. Please try again.\n");
            back();
        }
    }
}

int main()
{
    menu();
    return 0;
}
