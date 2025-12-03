// alloc.h
#ifndef ALLOC_H
#define ALLOC_H

void contiguousAllocate(char filenames[][50], int sizes[], int nFiles, int totalBlocks);
void linkedAllocate(char filenames[][50], int sizes[], int nFiles, int totalBlocks);
void indexedAllocate(char filenames[][50], int sizes[], int nFiles, int totalBlocks);

#endif
