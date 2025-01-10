# README 
## Overview
The `maint.c` program is a file management simulator that allows users to create, manage, and manipulate files using various allocation methods such as contiguous and chained organization. The simulator includes functionality for adding records, searching, deleting, renaming files, and visualizing memory usage.

---

## How to Compile the Program

1. Open a terminal or command prompt.
2. Ensure you have a C compiler (e.g., GCC) installed.
3. Navigate to the directory containing `main.c`.
4. Run the following command to compile the program:

   ```bash
   gcc main.c -o main
   ```

5. After successful compilation, an executable named `MainProject` (or `MainProject.exe` on Windows) will be created.

---

## How to Run the Program

1. Execute the compiled file using the following command:

   ```bash
   ./main
   ```

2. Follow the on-screen menu to navigate through various options.

---

## Menu Options

### 1. **Display the Menu**

![Insert Record Output](https://media.discordapp.net/attachments/1300606745382617118/1327375358458204231/Screenshot_2025-01-10_213138.png?ex=6782d622&is=678184a2&hm=3f1cebe8f9e99bd27a52110fcc423d9eab74eb028b7c3cd60ec661a71baf594d&=&format=webp&quality=lossless&width=854&height=374)

### 2. **Create file**
   Create a new file by choosing its organization type (Contiguous or Chained) and whether it is sorted or unsorted.

![Insert Record Output](https://media.discordapp.net/attachments/1300606745382617118/1327375649450758165/Screenshot_2025-01-10_213217.png?ex=6782d668&is=678184e8&hm=fe5068b93083a5cad51cdceb024fa6085ce6f3399717be218c9c9132571cd7dd&=&format=webp&quality=lossless&width=627&height=244)


### 3. **Delete file**
  

   ![Insert Record Output](https://media.discordapp.net/attachments/1300606745382617118/1327380163737944074/Screenshot_2025-01-10_215434.png?ex=6782da9c&is=6781891c&hm=a182f90a6288f719a22cad43699c6d2a6053754cb3ed56df0a0e8d141e45d21b&=&format=webp&quality=lossless&width=615&height=374)


### 4. **Rename file**
   Rename a file by providing the old name and a new name.

 ![Insert Record Output](https://media.discordapp.net/attachments/1300606745382617118/1327376503859580998/Screenshot_2025-01-10_213445.png?ex=6782d733&is=678185b3&hm=acfa1a61108de1a99efcb6dd3722f70444f6a2ade4d6cce6e8f0b7da8c4705e8&=&format=webp&quality=lossless&width=627&height=216)


### 5. **Display Memory state **
   Display the current memory allocation table, showing occupied and free blocks.

  ![Insert Record Output](https://media.discordapp.net/attachments/1300606745382617118/1327375945006321664/Screenshot_2025-01-10_213324.png?ex=6782d6ae&is=6781852e&hm=6aacede79e6c552025d327a33a50649f3fc412156001fb9bac004ea02da31812&=&format=webp&quality=lossless&width=635&height=374)


### 6. **add a record**
   Add a record (ID and Name) to an existing file.
   
![Insert Record Output](https://media.discordapp.net/attachments/1300606745382617118/1327376180743245986/Screenshot_2025-01-10_213406.png?ex=6782d6e6&is=67818566&hm=d461ad7a1b677e1e340baf9d9056c46ca70d85e2c49862a5ff8c90170b58db19&=&format=webp&quality=lossless&width=523&height=168)


### 7. **Display the record in file **
   
   ![Insert Record Output](https://media.discordapp.net/attachments/1300606745382617118/1327376503859580998/Screenshot_2025-01-10_213445.png?ex=6782d733&is=678185b3&hm=acfa1a61108de1a99efcb6dd3722f70444f6a2ade4d6cce6e8f0b7da8c4705e8&=&format=webp&quality=lossless&width=627&height=216)


### 8. **Logical Delete record**

   ![Insert Record Output](https://media.discordapp.net/attachments/1300606745382617118/1327377117276541018/Screenshot_2025-01-10_214206.png?ex=6782d7c6&is=67818646&hm=745049bb5af3a44ea927a83e27543b8a9f46d1b2fe6c00058b4c1b43eab2cb0a&=&format=webp&quality=lossless&width=799&height=216)


### 9. **Physical Delete record**

   ![Insert Record Output](https://media.discordapp.net/attachments/1300606745382617118/1327377117276541018/Screenshot_2025-01-10_214206.png?ex=6782d7c6&is=67818646&hm=745049bb5af3a44ea927a83e27543b8a9f46d1b2fe6c00058b4c1b43eab2cb0a&=&format=webp&quality=lossless&width=799&height=216)


### 10. **Memory comptage**
 
  ![Insert Record Output](https://media.discordapp.net/attachments/1300606745382617118/1327380529045176410/Screenshot_2025-01-10_215526.png?ex=6782daf3&is=67818973&hm=d9c7243e84d922c310528384e8fc633f70e2ce69b396e7c0e368ccc5db027e28&=&format=webp&quality=lossless&width=702&height=374)


### 11. **Display metadata**
 
![Insert Record Output](https://media.discordapp.net/attachments/1300606745382617118/1327379675240202260/Screenshot_2025-01-10_215223.png?ex=6782da28&is=678188a8&hm=fc32d583da3f24f199d18d607c67242237c72c00a672886a2ae3bdf751ad5bbb&=&format=webp&quality=lossless&width=675&height=240)




### 12. **Memory clean**
   

   ![Insert Record Output](https://media.discordapp.net/attachments/1300606745382617118/1327380943391952936/Screenshot_2025-01-10_215655.png?ex=6782db56&is=678189d6&hm=ba0f8bdec81896915c7fad2b6bd1d5efb8d21c63c3e653e8c61cadf078c3ba2f&=&format=webp&quality=lossless&width=650&height=374)





### 13. **Exiting the program**

![Insert Record Output](https://media.discordapp.net/attachments/1300606745382617118/1327381003110584400/Screenshot_2025-01-10_215723.png?ex=6782db64&is=678189e4&hm=3d095006166b89b0a9639cf770304ada0fb166db5f5d0d672a991e92ea3a5174&=&format=webp&quality=lossless&width=580&height=374)








---

## Important Functions

### File Creation
- **create_file()**
  - Allows users to create a file in contiguous or chained organization.
  - Prompts for file name, organization type, and sorted/unsorted state.

### Adding Records
- **add_record_based_on_type()**
  - Adds records to an existing file based on its organization type.

### Displaying Memory Allocation
- **display_allocation_table()**
  - Displays the current memory layout with free and occupied blocks.

### File Deletion
- **delete_file(char *file_name)**
  - Deletes a file and frees its associated memory.

### Renaming Files
- **rename_file(char *old_name, char *new_name)**
  - Renames an existing file.

### Memory Initialization
- **initialize_allocation_table()**
  - Initializes the memory by marking all blocks as free.

---

## Error Handling
- Invalid inputs are handled by prompting the user to enter valid data.
- Messages are displayed for successful or failed operations.

---




