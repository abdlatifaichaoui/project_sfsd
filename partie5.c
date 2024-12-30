#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_BLOCKS 100

typedef struct MetaData {
    char fileName[50];
    int fileSize;
    int usedBlocks;
    int recordCount;
    int firstBlockAddress;
} MetaData;

typedef struct Block {
    int id;
    char data[256];
    int next;
} Block;

// Read metadata
void readMetaData(FILE *f, MetaData *meta) {
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

void addRecord(FILE *fMeta, FILE *fBlocks, const char *data, int dataSize) {
    MetaData meta;
    readMetaData(fMeta, &meta);

    Block block;
    fseek(fBlocks, meta.firstBlockAddress * sizeof(Block), SEEK_SET);
    fread(&block, sizeof(Block), 1, fBlocks);

    // If the current block is full, add a new one
    while (strlen(block.data) + dataSize > sizeof(block.data)) {
        if (block.next == -1) {
            block.next = meta.usedBlocks;
            fseek(fBlocks, meta.firstBlockAddress * sizeof(Block), SEEK_SET);
            fwrite(&block, sizeof(Block), 1, fBlocks);

            Block newBlock = {meta.usedBlocks, "", -1};
            meta.usedBlocks++;
            fseek(fBlocks, newBlock.id * sizeof(Block), SEEK_SET);
            fwrite(&newBlock, sizeof(Block), 1, fBlocks);
            block = newBlock;
        } else {
            fseek(fBlocks, block.next * sizeof(Block), SEEK_SET);
            fread(&block, sizeof(Block), 1, fBlocks);
        }
    }

    strcat(block.data, data);
    strcat(block.data, "\n");
    fseek(fBlocks, block.id * sizeof(Block), SEEK_SET);
    fwrite(&block, sizeof(Block), 1, fBlocks);

    meta.recordCount++;
    updateMetaData(fMeta, &meta);
}

int main() {
    FILE *fMeta = fopen("MetaData.bin", "wb+");
    FILE *fBlocks = fopen("Blocks.bin", "wb+");

    if (!fMeta || !fBlocks) {
        printf("Error opening files\n");
        return 1;
    }

    createMetaData(fMeta, "DataFile.bin", 1024, 1, 0);

    MetaData meta;
    readMetaData(fMeta, &meta);
    displayMetaData(&meta);

    addRecord(fMeta, fBlocks, "Product A", 9);
    addRecord(fMeta, fBlocks, "Product B", 9);

    readMetaData(fMeta, &meta);
    displayMetaData(&meta);

    fclose(fMeta);
    fclose(fBlocks);

    return 0;
}
