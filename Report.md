# Report on Data Structures and Algorithms

## 1. Introduction

This report provides a detailed overview of the data structures and algorithms implemented in the provided project. It includes a description of the chosen methods, their purposes, and the results of tests conducted to ensure their functionality. Screenshots illustrating the functionality are also included.

---

## 2. Data Structures and Algorithms Used

### 2.1 Data Structures

The project employs several key data structures to manage and manipulate data effectively. Below is a description of the main structures used:

#### 2.1.1 Arrays

Arrays are utilized to store a collection of related elements, such as sequences of integers or characters. They are chosen for their simplicity and constant-time access to elements by index.

**Purpose**: Arrays are used in sections where fixed-size collections are needed, such as managing specific configurations or storing pre-defined inputs.

#### 2.1.2 Structs

Structs in C are used to define complex data types that group multiple fields. In this project, structs are used to represent entities such as `Record`, `Node`, or other constructs.

**Purpose**: To organize related data logically, ensuring code readability and ease of manipulation.

**Example**: A `Node` struct might be defined as follows:

```c
typedef struct Node {
    int id;
    char name[MAX_NAME_LENGTH];
    struct Node* next;
} Node;
```

#### 2.1.3 Linked Lists

Linked lists are implemented to allow dynamic memory allocation and flexible data storage. Each node in the list points to the next, forming a chain.

**Purpose**: Linked lists are particularly useful for operations involving frequent insertions and deletions, such as in chained file organization.

---

### 2.2 Algorithms

#### 2.2.1 Record Insertion Algorithms

The project features algorithms for inserting records into both contiguous and chained file organizations.

- **Contiguous File Insertion**: Handles sorted and unsorted records. Sorted insertion involves shifting elements to maintain order.
- **Chained File Insertion**: Dynamically allocates memory for new nodes, maintaining sorted or unsorted order as specified.

**Example**: Inserting into a chained file:

```c
void add_record_chained(ChainedFile *file, int id, const char *name) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->record.id = id;
    strcpy(new_node->record.name, name);
    new_node->next = NULL;

    if (file->sorted) {
        // Insert in sorted order
        if (file->head == NULL || file->head->record.id >= id) {
            new_node->next = file->head;
            file->head = new_node;
        } else {
            Node *current = file->head;
            while (current->next != NULL && current->next->record.id < id) {
                current = current->next;
            }
            new_node->next = current->next;
            current->next = new_node;
        }
    } else {
        // Insert at the head for unsorted order
        new_node->next = file->head;
        file->head = new_node;
    }
}
```

#### 2.2.2 Memory Management Algorithms

The project implements a memory allocation table to track block usage within a virtual disk. Functions include:

- **Allocation Table Initialization**: Marks all blocks as free during initialization.
- **Defragmentation**: Consolidates fragmented blocks to optimize memory usage.
- **Compact Memory**: Reorganizes memory blocks to free up contiguous spaces.

**Example**: Visualizing memory usage:

```c
void display_allocation_table() {
    for (int i = 0; i < NUM_BLOCKS; i++) {
        if (!virtual_disk[i].is_occupied) {
            printf("[Block %d: Free] ", i);
        } else {
            printf("[Block %d: Occupied] ", i);
        }
    }
}
```

---

## 3. Test Results

### 3.1 Test Environment

All tests were conducted on a system with the following configuration:

- **Operating System**: Ubuntu 20.04
- **Compiler**: GCC version 9.3.0
- **Test Data**: Sample data sets designed to evaluate functionality under various scenarios.

---

### 3.2 Test Scenarios and Results

#### 3.2.1 File Creation Test

**Objective**: To verify file creation functionality for both contiguous and chained file types.

**Input**: Create a chained file named `File1` (sorted).

**Output**: Confirmation message and memory block allocation.



#### 3.2.2 Record Insertion Test

**Objective**: To validate correct insertion of records into files.

**Input**: Add records with IDs `101`, `102`, and `103` to `File1`.

**Output**: Records inserted in sorted order:

```
Record 1: ID=101
Record 2: ID=102
Record 3: ID=103
```



#### 3.2.3 Memory Visualization Test

**Objective**: To ensure accurate visualization of memory block usage.

**Input**: Display the allocation table after creating two files.

**Output**:

```
[Block 0: Occupied] [Block 1: Free] ... [Block 99: Free]
```



---

## 4. Conclusion

The project demonstrates effective use of data structures and algorithms to solve specific problems related to file organization and memory management. Test results confirm that the implementations are functional and meet the expected requirements. Screenshots included illustrate the correct operation of key functionalities.

Further improvements could involve optimizing the algorithms for better performance or extending the functionality to handle larger datasets and additional file operations.

---

**End of Report**

