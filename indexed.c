// indexed.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alloc.h"

void print_disk_map_indexed(char filenames[][50], int owner[], int totalBlocks) {
    printf("\nDisk map (block : content)\n");
    for (int i = 0; i < totalBlocks; ++i) {
        if (owner[i] == -1) printf("%2d: .  ", i);
        else printf("%2d:%.6s  ", i, filenames[owner[i]]);
    }
    printf("\n");
}

void indexedAllocate(char filenames[][50], int sizes[], int nFiles, int totalBlocks) {
    if (totalBlocks > 1000) totalBlocks = 1000;
    int owner[1000];
    for (int i = 0; i < totalBlocks; ++i) owner[i] = -1;

    int **indexBlocks = malloc(sizeof(int*) * nFiles);
    int indexSizes[100];
    for (int f = 0; f < nFiles; ++f) { indexBlocks[f] = NULL; indexSizes[f] = 0; }

    printf("Indexed Allocation\n\n");

    for (int f = 0; f < nFiles; ++f) {
        int need = sizes[f];
        if (need == 0) { printf("%s : size 0 (no blocks)\n", filenames[f]); continue; }

        int freeCount = 0;
        for (int b = 0; b < totalBlocks; ++b) if (owner[b] == -1) freeCount++;
        if (freeCount < need) {
            printf("%s -> Allocation failed (not enough free blocks)\n", filenames[f]);
            continue;
        }

        indexBlocks[f] = malloc(sizeof(int) * need);
        int filled = 0;
        for (int b = 0; b < totalBlocks && filled < need; ++b) {
            if (owner[b] == -1) {
                owner[b] = f;
                indexBlocks[f][filled++] = b;
            }
        }
        indexSizes[f] = filled;
        printf("%s index -> [", filenames[f]);
        for (int i = 0; i < indexSizes[f]; ++i) {
            printf("%d", indexBlocks[f][i]);
            if (i != indexSizes[f]-1) printf(", ");
        }
        printf("]\n");
    }

    print_disk_map_indexed(filenames, owner, totalBlocks);

    for (int f = 0; f < nFiles; ++f) if (indexBlocks[f]) free(indexBlocks[f]);
    free(indexBlocks);
}
