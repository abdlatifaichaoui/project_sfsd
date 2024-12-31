#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_BLOCKS 10 
#define RED "\033[0;31m"
#define RESET "\033[0m"
#define GREEN "\033[0;32m"
#define NUM_BLOCKS 100
#define BLOCK_SIZE 512
#define MAX_FILENAME_LENGTH 15
#define MAX_FILES 10
#define MAX_SIZE 10
#define BLOCKS_PER_ROW 5
#define MAX_NAME_LENGTH 15

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH ];
} Record;

// Chained organization
typedef struct Node {
    Record record;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    int sorted; // 1 if sorted, 0 otherwise
} ChainedFile;

typedef struct {
    Record records[MAX_SIZE];
    int current_size;
    int sorted; // 1 if sorted, 0 otherwise
} ContiguousFile;

typedef struct {
    ContiguousFile contiguous_files[MAX_FILES];
    ChainedFile chained_files[MAX_FILES];
    char file_names[MAX_NAME_LENGTH]; // Store file names
    int file_count;
     int is_occupied;     // 0 = Free, 1 = Occupied
} SecondaryMemory;

SecondaryMemory virtual_disk[NUM_BLOCKS]; // Virtual disk with blocks


void initialize_contiguous_sorted_file(ContiguousFile *file) {
    file->current_size = 0;
    file->sorted = 1;
}
void initialize_contiguous_unsorted_file(ContiguousFile *file) {
    file->current_size = 0;
    file->sorted = 0; // Files are unsorted initially
}

void add_record_contiguous(ContiguousFile *file, int id, const char *name[]) {
    if (file->current_size >= MAX_SIZE) {
        printf("Error: Insufficient space in the contiguous file.\n");
        
    }

    if (file->sorted) {
        // Insertion in a sorted file
        int i = file->current_size - 1;
        while (i >= 0 && file->records[i].id > id) {
            file->records[i + 1] = file->records[i]; // Shift records to the right
            i--;
        }
        file->records[i + 1].id = id;
        strcpy(file->records[i + 1].name, name);
        file->current_size++;

        // Check if the file remains sorted
        if (i + 1 < file->current_size - 1 && file->records[i + 1].id > file->records[i + 2].id) {
            file->sorted = 0; // Mark as unsorted
        }
    } else {
        // Insertion in an unsorted file
        file->records[file->current_size].id = id;
        strcpy(file->records[file->current_size].name, name);
        file->current_size++;
    }

}

void display_contiguous_file(const ContiguousFile *file) {
    if (file->current_size == 0) {
        printf("The contiguous file is empty.\n");
    } else {
        printf("Sorted: %s\n", file->sorted ? "Yes" : "No");
        int i;
        for ( i = 0; i < file->current_size; i++) {
            printf("Record %d: ID=%d, Name=%s\n", i + 1, file->records[i].id, file->records[i].name);
        }
    }
}

// Functions for chained organization
void initialize_chained_sorted_file(ChainedFile *file) {
    file->head = NULL;
    file->sorted = 1; // Files are sorted initially
}

void initialize_chained_unsorted_file(ChainedFile *file) {
    file->head = NULL;
    file->sorted = 0; // Files are sorted initially
}

void add_record_chained(ChainedFile *file, int id, const char *name[]) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        printf("Error: Insufficient memory.\n");

    }
    new_node->record.id = id;
    strcpy(new_node->record.name, name);
    new_node->next = NULL;

    if (file->sorted) {
        // Insertion in a sorted file
        if (file->head == NULL || file->head->record.id >= id) {
            // Insert at the beginning
            new_node->next = file->head;
            file->head = new_node;
        } else {
            Node *current = file->head;
            while (current->next != NULL && current->next->record.id < id) {
                current = current->next;
            }
            // Insert after the current node
            new_node->next = current->next;
            current->next = new_node;
        }

        // Check if the file remains sorted
        Node *current = file->head;
        while (current != NULL && current->next != NULL) {
            if (current->record.id > current->next->record.id) {
                file->sorted = 0; // Mark as unsorted
                break;
            }
            current = current->next;
        }
    } else {
        // Insertion in an unsorted file
        new_node->next = file->head;
        file->head = new_node;
    }
}

void display_chained_file(const ChainedFile *file) {
    if (file->head == NULL) {
        printf("The chained file is empty.\n");
    } else {
        printf("Sorted: %s\n", file->sorted ? "Yes" : "No");
        Node *current = file->head;
        while (current != NULL) {
            printf("Record: ID=%d, Name=%s\n", current->record.id, current->record.name);
            current = current->next;
        }
    }
}

// File creation
void create_file() {
    int type;
   int sorted_option;
    char file_name[MAX_NAME_LENGTH];
    int block_found = 0;

    // Search for the first available block in virtual_disk
    int i;
    for ( i = 0; i < NUM_BLOCKS; i++) {
        if (virtual_disk[i].is_occupied == 0) {  // If block is free
            block_found = 1;
            virtual_disk[i].is_occupied = 1;  // Mark this block as occupied
            printf("Using block %d for the new file.\n", i);

            // Ask user for the file name
            printf("Enter the name of the file: ");
            scanf("%s", file_name);

            // Ask if the file is sorted or unsorted
            do {
			
            printf("Should the file be sorted initially? \n ");
            printf("1. Yes \n");
            printf("2. No \n");
             if (scanf("%d", &sorted_option) != 1) {
            // If scanf fails, flush the input buffer and prompt again
            while (getchar() != '\n');  // Clear the input buffer
            printf("Invalid input. Please enter 1 for Yes or 2 for No.\n");
        } else if (sorted_option != 1 && type != 2) {
            printf("Invalid input. Please enter 1 for Yes or 2 for No.\n");
        }

    } while (sorted_option != 1 && sorted_option != 2);

            // Ask for the organization type (contiguous or chained)
            do{
			
            printf("Organization type : \n");
            printf("1.Chained \n");
            printf("2.Contiguous \n");
            
             if (scanf("%d", &type) != 1) {
            // If scanf fails, flush the input buffer and prompt again
            while (getchar() != '\n');  // Clear the input buffer
            printf("Invalid input. Please enter 1 for Chained or 2 for Contiguous.\n");
        } else if (type != 1 && type != 2) {
            printf("Invalid input. Please enter 1 for Chained or 2 for Contiguous.\n");
        }

    } while (type != 1 && type != 2);

            // Create the file based on the type chosen by the user
            if (type == 2) {
            if (sorted_option == 1){
            	// Initialize the contiguous sorted file
                initialize_contiguous_sorted_file(&virtual_disk[i].contiguous_files[virtual_disk[i].file_count]);

                // Add the file name and initialize the first record
                strcpy(virtual_disk[i].file_names, file_name);
                add_record_contiguous(&virtual_disk[i].contiguous_files[virtual_disk[i].file_count], 1, "First Record");

                printf("Contiguous file '%s' created successfully.\n", file_name);
			}else {if (sorted_option == 2){
            		// Initialize the contiguous unsorted file
                initialize_contiguous_unsorted_file(&virtual_disk[i].contiguous_files[virtual_disk[i].file_count]);

                // Add the file name and initialize the first record
                strcpy(virtual_disk[i].file_names, file_name);
                add_record_contiguous(&virtual_disk[i].contiguous_files[virtual_disk[i].file_count], 1, "First Record");

                printf("Contiguous file '%s' created successfully.\n", file_name);
            		
				}else {
					printf("Coix invalide \n");
				}
			}
				
                
            } else if ( type == 1) {
            	if (sorted_option == 1){
				 // Initialize the chained sorted file
                initialize_chained_sorted_file(&virtual_disk[i].chained_files[virtual_disk[i].file_count]);

                // Add the file name and initialize the first record
                strcpy(virtual_disk[i].file_names, file_name);
                add_record_chained(&virtual_disk[i].chained_files[virtual_disk[i].file_count], 1, "First Record");

                printf("Chained file '%s' created successfully.\n", file_name);
				}else{if(sorted_option ==2){ 
				// Initialize the chained unsorted file
                initialize_chained_unsorted_file(&virtual_disk[i].chained_files[virtual_disk[i].file_count]);

                // Add the file name and initialize the first record
                strcpy(virtual_disk[i].file_names, file_name);
                add_record_chained(&virtual_disk[i].chained_files[virtual_disk[i].file_count], 1, "First Record");

                printf("Chained file '%s' created successfully.\n", file_name);
				}else{
						printf("Coix invalide \n");
				}
				}
               
            } else {
                printf("Invalid organization type.\n");
                return;
            }

            // Increment the file count to track the new file in the block
            virtual_disk[i].file_count++;
            break;
        }
    }

    if (!block_found) {
        printf("Error: No free block available in the virtual disk.\n");
    }
}


void add_record_based_on_type() {
	
	char file_name[MAX_FILENAME_LENGTH];
	int id ;
	 char name[MAX_NAME_LENGTH ];
	
	printf("Donnez le nom de fichier : \n");
	scanf("%s",&file_name);
	do{
	printf("Donnez le ID : \n");
	 if (scanf("%d", &id) != 1) {
            // If scanf fails, flush the input buffer and prompt again
            while (getchar() != '\n');  // Clear the input buffer
            printf("Invalid input. \n");
        } 
    } while( scanf("%d", &id) != 1);
	printf("Donnez le nom : \n");
	scanf("%s",&name);
	
	
    // Find the file corresponding to the file_name
    int found = 0;
    int i;
    for ( i = 0; i < NUM_BLOCKS; i++) {
    	int j;
        for ( j = 0; j < virtual_disk[i].file_count; j++) {
            if (strcmp(virtual_disk[i].file_names, file_name) == 0) {
                // File found, now identify its type (contiguous or chained)
                found = 1;
                // Check if the file is Contiguous
                if (virtual_disk[i].contiguous_files[j].current_size > 0) {
                    // It is a contiguous file
                        add_record_contiguous(&virtual_disk[i].contiguous_files[j], id, name);
                        printf("L'enregistrement a ete ajoute avec succes \n");
                    
                }
                // Check if the file is Chained
                else if (virtual_disk[i].chained_files[j].head != NULL) {
                    // It is a chained file
                    
                        add_record_chained(&virtual_disk[i].chained_files[j], id, name);
                          printf("L'enregistrement a ete ajoute avec succes \n");
                    
                }
                break;  // Exit the inner loop since the file has been found
            
            }
        }
        if (found) {
		break; 
		} // Exit the outer loop as well
    }

    if (!found) {
        printf("Error: File '%s' not found.\n", file_name);
    }
}




int back(){

	
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

void initialize_allocation_table() {
	int i;
    for (i = 0; i < NUM_BLOCKS; i++) {
        virtual_disk[i].is_occupied = 0; // All blocks are free at initialization
        memset(virtual_disk[i].contiguous_files, 0, BLOCK_SIZE); // Clear block data
         memset(virtual_disk[i].chained_files, 0, BLOCK_SIZE);
    }
}
void display_allocation_table() {
    printf("\n--- Memory Visualization ---\n");
int i;
    for (i = 0; i < NUM_BLOCKS; i++) {
        if (!virtual_disk[i].is_occupied) {
            printf(GREEN "[Block %d:  Free   ]" RESET " ", i);
        } else {
            printf(RED "[Block %d:Occuppied]" RESET " ", i);
        }

        // Break into rows for better readability
        if ((i + 1) % BLOCKS_PER_ROW == 0) {
            printf("\n");
        }
    }
    printf("\n");
    back();
}


void rename_file(char *old_name, char *new_name) {
  
    int file_found = 0;


    // Search for the file in all blocks
    int i;
    for ( i = 0; i < NUM_BLOCKS; i++) {
        if (virtual_disk[i].is_occupied == 1) { // Check if the block is occupied
            // Search through contiguous files in this block
            int j;
            for ( j = 0; j < virtual_disk[i].file_count; j++) {
                if (strcmp(virtual_disk[i].file_names, old_name) == 0) { // File found
                    strcpy(virtual_disk[i].file_names, new_name); // Rename the file
                    printf("File renamed from '%s' to '%s' successfully.\n", old_name, new_name);
                    file_found = 1;
                    break;
                }
            }

        }
    }

    if (!file_found) {
        printf("Error: File with name '%s' not found.\n", old_name);
    }
}


// Delete a file
void delete_file(char *file_name) {
  
    int file_found = 0;

   

    // Search for the file in all blocks
    int i;
    for ( i = 0; i < NUM_BLOCKS; i++) {
        if (virtual_disk[i].is_occupied == 1) { // Check if the block is occupied
            // Search through contiguous files in this block
            int j;
            for ( j= 0; j < virtual_disk[i].file_count; j++) {
                if (strcmp(virtual_disk[i].file_names, file_name) == 0) { // File found
                    // Mark the block as free and reset the file data
                    virtual_disk[i].is_occupied = 0;  // Mark the block as free
                    memset(&virtual_disk[i].contiguous_files[j], 0, sizeof(ContiguousFile)); // Clear file data
                    memset(&virtual_disk[i].chained_files[j], 0, sizeof(ChainedFile)); // Clear file data
                    printf("File '%s' deleted successfully.\n", file_name);
                    file_found = 1;
                    break;
                }
            }

           
        }

        if (file_found) {
            break;
        }
    }

    if (!file_found) {
        printf("Error: File with name '%s' not found.\n", file_name);
      
    }
}




void showMenu() {
    printf("\nMenu:\n\n");
    printf("1. Creer un fichier \n");
    printf("2. Inserer un enregestrement \n");
    printf("3. Recherch un enregestrement par ID \n");
    printf("4. Suppremer un enregestrement par ID\n");
    printf("5. Supprimer un fichier\n");
    printf("6. Defragmentation d'un fichier \n");
    printf("7. Afficher le fichier de metadonnées associe à un fichier\n");
    printf("8. Compactage de la memoire secondaire\n");
    printf("9. Renomer un fichier \n");
    printf("10. Affichier l'etat de la memoire \n");
    printf("11. Vider la memoire\n");
    printf("12. Quitter le programme\n");
}
void exitProgram() {
    printf("Quitter le programme...\n");
exit(0);
}


int main() {

	initialize_allocation_table();
   int choice;

    while (1) {
        showMenu();
        printf("Entrez votre choix: ");
    if (scanf("%d", &choice) != 1) {
            
            while (getchar() != '\n') // Clear the buffer
            printf("Invalid choice. Please enter a number between 1 and 12.\n");
     	back();
     	 
        }else{
		

        switch (choice) {
            case 1:
            		system("cls");
			 printf("creer un fichier \n"); 
			 create_file();
			 back(); break;
            case 2:
            		system("cls");
			 printf("inserer un enregestrement \n"); 
			 add_record_based_on_type();
			 back(); break;
            case 3: 
            	system("cls");
			printf("recherche enregestrement \n"); 
			back(); break;
            case 4: 
            	system("cls");
			printf("suppremer un enregestrement\n");
			back(); break;
            case 5: 
            	system("cls");
            	char filename[MAX_FILENAME_LENGTH];
			printf("donnez le nome de fichier que tu veux suppremer \n");
			scanf("%s",filename);
			delete_file(filename);
			 back(); break;
            case 6:
            		system("cls");
			 printf("defragmentation \n"); 
			 back(); break;
            case 7:
            		system("cls");
			 printf("affichier metadoone\n");
			  back(); break;
            case 8: 
            	system("cls");
			printf("compactage de MS \n"); 
			back(); break;
            case 9: 
            	system("cls");
            	char oldname[MAX_FILENAME_LENGTH ],newname[MAX_FILENAME_LENGTH ];
			printf("renomer un fichier: \n \n");
			 printf("Entrez le nom de fichier que tu veux renomer: \n");
			 scanf("%s",oldname);
			 printf("Enter le nouveau nom : \n ");
			 scanf("%s",newname);	
			rename_file(oldname,newname);
			 back(); break;
            case 10:
            		system("cls");
			 printf("L'etat De Memoire : \n"); 
			 display_allocation_table();
			  break;
            case 11: 
            	system("cls");
			printf("vider la memoire \n");
			back();  break;
			case 12: exitProgram(); return 0;
            default: printf("Choix invalide. Essayez encore.\n"); back(); break;
        }}
    }

    return 0;
}
   

   
