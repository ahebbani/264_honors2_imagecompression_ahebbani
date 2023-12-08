// huffman.h

#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>

// Structure to represent a Huffman tree node
struct MinHeapNode {
    char data;
    unsigned freq;
    struct MinHeapNode *left, *right;
};

// Function to compress the input text file and write to the output compressed file
void compressFile(const char *inputFileName, const char *outputFileName);

// Function to decompress the input compressed file and write to the output decompressed file
void decompressFile(const char *inputFileName, const char *outputFileName);

#endif /* HUFFMAN_H */
