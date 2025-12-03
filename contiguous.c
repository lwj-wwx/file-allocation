// contiguous.c
#include <stdio.h>
#include <string.h>
#include "alloc.h"

void print_disk_map_contiguous(char filenames[][50], int owner[], int totalBlocks) {
    printf("\nDisk map (block : content)\n");
    for (int i = 0; i < totalBlocks; ++i) {
        if (owner[i] == -1) printf("%2d: .  ", i);
        else printf("%2d:%.6s  ", i, filenames[owner[i]]); // show up to 6 chars
    }
    printf("\n");
}

void contiguousAllocate(char filenames[][50], int sizes[], int nFiles, int totalBlocks) {
    if (totalBlocks > 1000) totalBlocks = 1000;
    int owner[1000];
    for (int i = 0; i < totalBlocks; ++i) owner[i] = -1;

    printf("Contiguous Allocation\n\n");
    for (int f = 0; f < nFiles; ++f) {
        int need = sizes[f];
        if (need == 0) { printf("%s : size 0 (no blocks)\n", filenames[f]); continue; }
        int found = 0;
        for (int start = 0; start <= totalBlocks - need; ++start) {
            int ok = 1;
            for (int k = 0; k < need; ++k) if (owner[start + k] != -1) { ok = 0; break; }
            if (ok) {
                for (int k = 0; k < need; ++k) owner[start + k] = f;
                printf("%s -> blocks %d to %d\n", filenames[f], start, start + need - 1);
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("%s -> Allocation failed (no contiguous space for %d blocks)\n", filenames[f], need);
        }
    }

    print_disk_map_contiguous(filenames, owner, totalBlocks);
}
