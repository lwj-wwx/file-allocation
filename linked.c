#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*This program reads file names and block counts from "sizes.txt" and simulates linked allocation for each file.*/

struct Block {
    int blockNum;           // Block number in simulated disk
    struct Block *next;     // Pointer to next block
};

struct FileEntry {
    char name[100];
    int blocksNeeded;       // How many blocks the file needs
    struct Block *start;    // Linked list head for this file
};

int main() {
    FILE *fp = fopen("sizes.txt", "r");
    if (!fp) {
        printf("Error: sizes.txt not found.\n");
        return 1;
    }

    struct FileEntry files[50];
    int fileCount = 0;
    printf("===LINKED FILE ALLOCATION SYSTEM===\n");

    // Read filename + block count from sizes.txt
    while (fscanf(fp, "%s %d", files[fileCount].name, &files[fileCount].blocksNeeded) != EOF) {
        fileCount++;
    }
    fclose(fp);

    printf("Loaded %d files from sizes.txt\n", fileCount);

    int nextFreeBlock = 0;  // Starting block number for simulation

    // Allocate linked blocks for each file
    for (int i = 0; i < fileCount; i++) {
        struct Block *head = NULL, *tail = NULL;
        for (int b = 0; b < files[i].blocksNeeded; b++) {
            // Create a new block
            struct Block *newBlock = (struct Block *)malloc(sizeof(struct Block));
            newBlock->blockNum = nextFreeBlock++;
            newBlock->next = NULL;
            // Insert into linked list
            if (head == NULL) {
                head = tail = newBlock;
            } else {
                tail->next = newBlock;
                tail = newBlock;
            }
        }
        files[i].start = head;
    }
    printf("\n===ALLOCATION RESULTS===\n");

    for (int i = 0; i < fileCount; i++) {
        printf("\nFile: %s\n", files[i].name);
        printf("Blocks Needed: %d\n", files[i].blocksNeeded);
        printf("Linked Allocation Path:\n");

        struct Block *temp = files[i].start;
        while (temp != NULL) {
            printf("[%d] -> ", temp->blockNum);
            temp = temp->next;
        }
        printf("NULL\n");
    }

    printf("\n===Allocation completed successfully===\n");
    return 0;
}
