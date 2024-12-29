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

#define BLOCKS_PER_ROW 5

typedef struct Block{
    int is_occupied;     // 0 = Free, 1 = Occupied
    char data[BLOCK_SIZE];
} Block;
typedef struct file {
	char name[15];
	int size;
	int first_block;
	int total_blocks;
}file;
file files[10];

Block virtual_disk[NUM_BLOCKS]; // Virtual disk with blocks
typedef struct {
   
    int num_records;
} BlockInfo;
int back(){

	
    char backKey;
    printf("Press any to go back to the main menu: ");
   

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
        memset(virtual_disk[i].data, 0, BLOCK_SIZE); // Clear block data
    }
}
void display_allocation_table() {
    printf("\n--- Memory Visualization ---\n");
int i;
    for (i = 0; i < NUM_BLOCKS; i++) {
        if (!virtual_disk[i].is_occupied) {
            printf(GREEN "[Block %d: Free]" RESET " ", i);
        } else {
            printf(RED "[Block %d]" RESET " ", i);
        }

        // Break into rows for better readability
        if ((i + 1) % BLOCKS_PER_ROW == 0) {
            printf("\n");
        }
    }
    printf("\n");
    back();
}


int rename_file( char *old_name, char *new_name) {
	int i;
    for ( i = 0; i < NUM_BLOCKS; i++) {
        if (files[i].name[0] != '\0' && strcmp(files[i].name, old_name) == 0) {
        
            strncpy(files[i].name, new_name, MAX_FILENAME_LENGTH);
            printf("File '%s' renamed to '%s'.\n", old_name, new_name);
            
            return 0;
        }
    }
    printf("Error: File '%s' not found.\n", old_name);
    return -1;
}

// Delete a file
int delete_file(const char *filename) {
	int i;
    for ( i = 0; i < 	NUM_BLOCKS; i++) {
        if (files[i].name[0] != '\0' && strcmp(files[i].name, filename) == 0) {
            int block = files[i].first_block;
            int j;
            for ( j = 0; j < files[i].total_blocks; j++) {
                	virtual_disk[block].is_occupied = 0;  // Mark blocks as free
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
			 back(); break;
            case 2:
            		system("cls");
			 printf("inserer un enregestrement \n"); 
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
   

   
