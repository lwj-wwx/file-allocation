#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 50       // maximum number of files
#define MAX_BLOCKS 1000    // total disk blocks
#define MAX_INDEX 100      // max blocks per file

typedef struct {
    char name[50];
    int size;
    int indexBlock[MAX_INDEX];
} File;

File files[MAX_FILES];
int fileCount = 0;
int freeBlock[MAX_BLOCKS]; // 1 = free, 0 = allocated

// Allocate blocks for a new file
void allocateFile(char *fileName, int size) {
    if(fileCount >= MAX_FILES) {
        printf("❌ File table full! Cannot allocate %s\n", fileName);
        return;
    }

    if(size > MAX_INDEX) {
        printf("⚠ File %s exceeds max index size (%d), truncating\n", fileName, MAX_INDEX);
        size = MAX_INDEX;
    }

    int allocated = 0;
    int block = 0;

    while(allocated < size) {
        if(freeBlock[block] == 1) {
            files[fileCount].indexBlock[allocated] = block;
            freeBlock[block] = 0;
            allocated++;
        }
        block = (block + 1) % MAX_BLOCKS; // wrap around
    }

    strcpy(files[fileCount].name, fileName);
    files[fileCount].size = size;
    fileCount++;
    printf("✔ File '%s' allocated (%d blocks)\n", fileName, size);
}

// Delete a file and free its blocks
void deleteFile(char *fileName) {
    int found = -1;
    for(int i = 0; i < fileCount; i++) {
        if(strcmp(files[i].name, fileName) == 0) {
            found = i;
            break;
        }
    }

    if(found == -1) {
        printf("❌ File '%s' not found\n", fileName);
        return;
    }

    // Free blocks
    for(int i = 0; i < files[found].size; i++) {
        freeBlock[files[i].indexBlock[i]] = 1;
    }

    // Remove file from array
    for(int i = found; i < fileCount - 1; i++) {
        files[i] = files[i+1];
    }
    fileCount--;
    printf("✔ File '%s' deleted\n", fileName);
}

// Print allocation table
void printAllocation() {
    if(fileCount == 0) {
        printf("(No files allocated)\n");
        return;
    }

    printf("\n=== Indexed Allocation Table ===\n\n");
    for(int i = 0; i < fileCount; i++) {
        printf("File: %s (%d blocks)\n", files[i].name, files[i].size);
        printf("Index block points to: ");
        for(int j = 0; j < files[i].size; j++)
            printf("%d ", files[i].indexBlock[j]);
        printf("\n\n");
    }
}

int main() {
    // Initialize all blocks as free
    for(int i = 0; i < MAX_BLOCKS; i++) freeBlock[i] = 1;

    // Read sizes.txt and allocate files initially
    FILE *fp = fopen("sizes.txt", "r");
    if(fp) {
        char name[50];
        int size;
        while(fscanf(fp, "%s %d", name, &size) == 2) {
            allocateFile(name, size);
        }
        fclose(fp);
    } else {
        printf("⚠ sizes.txt not found. Start with empty disk.\n");
    }

    int choice;
    char name[50];
    int size;

    while(1) {
        printf("\n=== Indexed Allocation Menu ===\n");
        printf("1. Show allocation table\n");
        printf("2. Allocate new file\n");
        printf("3. Delete a file\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                printAllocation();
                break;
            case 2:
                printf("Enter file name: ");
                scanf("%s", name);
                printf("Enter file size in blocks: ");
                scanf("%d", &size);
                allocateFile(name, size);
                break;
            case 3:
                printf("Enter file name to delete: ");
                scanf("%s", name);
                deleteFile(name);
                break;
            case 4:
                printf("Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}
