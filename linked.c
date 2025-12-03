// linked.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alloc.h"

void print_disk_map_linked(char filenames[][50], int owner[], int totalBlocks) {
    printf("\nDisk map (block : content)\n");
    for (int i = 0; i < totalBlocks; ++i) {
        if (owner[i] == -1) printf("%2d: .  ", i);
        else printf("%2d:%.6s  ", i, filenames[owner[i]]);
    }
    printf("\n");
}

void linkedAllocate(char filenames[][50], int sizes[], int nFiles, int totalBlocks) {
    if (totalBlocks > 1000) totalBlocks = 1000;
    int next[1000];
    int owner[1000];
    for (int i = 0; i < totalBlocks; ++i) { next[i] = -1; owner[i] = -1; }

    printf("Linked Allocation (chains)\n\n");
    int left = 0, right = totalBlocks - 1;
    int useLeft = 1;

    for (int f = 0; f < nFiles; ++f) {
        int need = sizes[f];
        if (need == 0) { printf("%s : size 0 (no blocks)\n", filenames[f]); continue; }

        int first = -1, prev = -1;
        int allocated = 0;
        for (int alloc = 0; alloc < need; ++alloc) {
            int chosen = -1;
            if (useLeft) {
                while (left <= right && owner[left] != -1) left++;
                if (left <= right) { chosen = left; left++; }
            } else {
                while (right >= left && owner[right] != -1) right--;
                if (right >= left) { chosen = right; right--; }
            }
            useLeft = !useLeft;

            if (chosen == -1) {
                for (int i = 0; i < totalBlocks; ++i) if (owner[i] == -1) { chosen = i; break; }
            }

            if (chosen == -1) {
                // not enough blocks, rollback
                printf("%s -> Allocation failed (not enough blocks)\n", filenames[f]);
                int cur = first;
                while (cur != -1) { owner[cur] = -1; cur = next[cur]; }
                // reset next entries that were part of this failed allocation
                for (int i = 0; i < totalBlocks; ++i) if (owner[i] == -1) next[i] = -1;
                first = -1;
                allocated = 0;
                break;
            }

            owner[chosen] = f;
            next[chosen] = -1;
            if (first == -1) first = chosen;
            if (prev != -1) next[prev] = chosen;
            prev = chosen;
            allocated++;
        }

        if (first != -1) {
            printf("%s chain head=%d : ", filenames[f], first);
            int cur = first;
            while (cur != -1) {
                printf("%d", cur);
                cur = next[cur];
                if (cur != -1) printf(" -> ");
            }
            printf("\n");
        } else if (allocated == 0) {
            // already printed failure
        }
    }

    print_disk_map_linked(filenames, owner, totalBlocks);
}
