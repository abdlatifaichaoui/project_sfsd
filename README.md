README
Project Name:
File Organization Simulator: Contiguous and Chained Storage Management

Description:
This program simulates file organization and management using both contiguous and chained storage techniques. It allows users to:

-Create, rename, and delete files.
-Add, search, and delete records in files.
-View and manage memory allocation for the virtual disk.
-Perform memory defragmentation and compaction.
-The program uses a virtual disk with a fixed number of blocks to simulate resource allocation.

Features:
1).Storage Organization:
 Contiguous Files: Store records in an array, sorted or unsorted.
 Chained Files: Use a linked list to store records, supporting sorted and unsorted modes.

2).File Operations:
Create a File: Choose organization type and sorting preference.
Add Records: Insert records into existing files.
Rename Files: Rename a file in the virtual disk.
Delete Files: Remove a file and free its associated memory block.

3).Record Operations:
Add, search, and delete records by ID.

4).Memory Management:
Display the memory allocation table.
Defragment and compact the memory to optimize storage usage.
Clear the entire memory.

5).User-Friendly Menu:
Provides a clean, interactive menu for managing all operations.

Setup and Compilation:
1.Clone the repository or download the source code.
2.Compile the program using a C compiler:

bash
     gcc -o MainProject MainProject.c

3.Run the compiled program:

bash
      ./MainProject

Menu Options
 Menu:
 1. Create a file
 2. Insert a record
 3. Search a record by ID
 4. Delete a record by ID
 5. Delete a file
 6. Defragment a file
 7. Display metadata associated with a file
 8. Compact secondary memory
 9. Rename a file
 10. Display memory state
 11. Clear memory
 12. Exit the program

Main Loop
The main() function runs an infinite loop displaying the menu and handling user input.

Invalid Inputs: Any invalid input is rejected, and the user is prompted again.
Exit Option: Selecting option 12 ends the program.

Function Details
1. Menu Display
The showMenu() function displays the menu for file and memory operations.

2. Exit Program
The exitProgram() function cleanly terminates the program.

3. Main Program Flow
The main() function:

Initializes the memory allocation table.
Continuously displays the menu.
Executes the operation corresponding to the user's choice

        
