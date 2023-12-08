#include <stdio.h>

#ifndef HUFFMAN_H
#define HUFFMAN_H

// Structure to represent a Huffman tree node
struct MinHeapNode {
    char data;
    unsigned freq;
    struct MinHeapNode *left, *right;
};

// Function to create a min heap
struct MinHeap {
    unsigned size;
    unsigned capacity;
    struct MinHeapNode **array;
};

// Function to compress the input text file and write to the output compressed file
void compressFile(const char *inputFileName, const char *outputFileName);

// Function to decompress the input compressed file and write to the output decompressed file
void decompressFile(const char *inputFileName, const char *outputFileName);

#endif /* HUFFMAN_H */
