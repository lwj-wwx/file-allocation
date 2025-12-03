// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alloc.h"

int main() {
    FILE *fp = fopen("sizes.txt", "r");
    if (!fp) {
        printf("sizes.txt not found!\n");
        return 0;
    }

    int totalBlocks;
    printf("Enter total disk blocks: ");
    if (scanf("%d", &totalBlocks) != 1 || totalBlocks <= 0) { fclose(fp); return 0; }

    char filenames[100][50];
    int sizes[100];
    int nFiles = 0;

    while (fscanf(fp, "%49s %d", filenames[nFiles], &sizes[nFiles]) == 2) {
        nFiles++;
        if (nFiles >= 100) break;
    }
    fclose(fp);

    if (nFiles == 0) {
        printf("No entries found in sizes.txt\n");
        return 0;
    }

    printf("\nRead %d entries from sizes.txt\n", nFiles);
    for (int i = 0; i < nFiles; ++i)
        printf("%2d) %s : %d blocks\n", i+1, filenames[i], sizes[i]);

    int choice;
    printf("\nChoose allocation method:\n1. Contiguous\n2. Linked\n3. Indexed\nChoice: ");
    if (scanf("%d", &choice) != 1) return 0;
    printf("\n");

    switch(choice) {
        case 1: contiguousAllocate(filenames, sizes, nFiles, totalBlocks); break;
        case 2: linkedAllocate(filenames, sizes, nFiles, totalBlocks); break;
        case 3: indexedAllocate(filenames, sizes, nFiles, totalBlocks); break;
        default: printf("Invalid choice\n");
    }

    return 0;
}
