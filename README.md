# File Organization Simulator: Contiguous and Chained Storage Management

## Description
This program simulates file organization and management using both **contiguous** and **chained storage techniques**. It allows users to:

- Create, rename, and delete files.
- Add, search, and delete records in files.
- View and manage memory allocation for the virtual disk.
- Perform memory defragmentation and compaction.
- Use a virtual disk with a fixed number of blocks to simulate resource allocation.

---

## Features

### 1. Storage Organization
- **Contiguous Files**: Store records in an array, sorted or unsorted.
- **Chained Files**: Use a linked list to store records, supporting sorted and unsorted modes.

### 2. File Operations
- **Create a File**: Choose the organization type and sorting preference.
- **Add Records**: Insert records into existing files.
- **Rename Files**: Rename a file in the virtual disk.
- **Delete Files**: Remove a file and free its associated memory block.

### 3. Record Operations
- Add, search, and delete records by ID.

### 4. Memory Management
- Display the memory allocation table.
- Defragment and compact the memory to optimize storage usage.
- Clear the entire memory.

### 5. User-Friendly Menu
- Provides a clean, interactive menu for managing all operations.

---

## Setup and Compilation

1. Clone the repository or download the source code.
2. Compile the program using a C compiler:
   ```bash
   gcc -o main main.c
3.Run the compiled program:
   ```
  ./main
   ```
---
## Students Members
- Afra Wafa (ID: 232331573420)
- Benaziza Malak (ID: 232331573215)
- Abdelatif Aichaoui (ID: 232331707304)
- Seddari Yassmine (ID: 232331414104)
- Khaznadji Yasmine (ID: 232331530104)
- Zerrouki Alaa (ID: 232331576503)
- Ainouz lyna (ID:232331551211)










