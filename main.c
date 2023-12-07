#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "compression.h"

#define MAX 16

int readArray(int fd, char arr[], int size) {
    return read(fd, arr, size);
}

int main(int argc, char *argv[]) {
    // Check if the input file name is provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    // Input file name provided as a command-line argument
    const char* inputFileName = argv[1];
    const char* outputSizesFileName = "output_sizes.txt";
    const char* compressedFileName = "compressed.bin";
    const char* decompressedFileName = "decompressed.txt";

    // Open input file
    int fdInput = open(inputFileName, O_RDONLY);
    if (fdInput == -1) {
        perror("Error opening input file");
        return 1;
    }

    // Measure the size of the input file
    off_t inputFileSize = lseek(fdInput, 0, SEEK_END);
    lseek(fdInput, 0, SEEK_SET);

    // Allocate memory for the input array
    char* inputArray = (char*)malloc(inputFileSize);
    if (inputArray == NULL) {
        perror("Memory allocation error");
        close(fdInput);
        return 1;
    }

    // Read the contents of the input file into the array
    int bytesRead = readArray(fdInput, inputArray, inputFileSize);
    if (bytesRead == -1) {
        perror("Error reading from input file");
        free(inputArray);
        close(fdInput);
        return 1;
    }

    // Create and build the Huffman tree
    int freq[MAX];  // Assuming MAX is defined in your compression.h
    // Initialize freq with the frequency of each character in inputArray
    struct Min_Heap* minHeap = createAndBuildMin_Heap(inputArray, freq, MAX);
    Node* huffmanTree = buildHuffmanTree(inputArray, freq, MAX, minHeap);

    // Compress the file
    int fdCompressed = open(compressedFileName, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fdCompressed == -1) {
        perror("Error opening compressed file");
        free(inputArray);
        close(fdInput);
        return 1;
    }
    compressFile(fdInput, fdCompressed, huffmanTree);

    // Measure the size of the compressed file
    off_t compressedFileSize = lseek(fdCompressed, 0, SEEK_END);
    lseek(fdCompressed, 0, SEEK_SET);

    // Decompress the file
    int fdDecompressed = open(decompressedFileName, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fdDecompressed == -1) {
        perror("Error opening decompressed file");
        free(inputArray);
        close(fdInput);
        close(fdCompressed);
        return 1;
    }
    ReBuildHuffmanTree(fdCompressed, MAX);  // Assuming ReBuildHuffmanTree function is defined
    decompressFile(fdCompressed, fdDecompressed, MAX, huffmanTree);

    // Measure the size of the decompressed file
    off_t decompressedFileSize = lseek(fdDecompressed, 0, SEEK_END);
    lseek(fdDecompressed, 0, SEEK_SET);

    // Write sizes to output file
    FILE* sizesFile = fopen(outputSizesFileName, "w");
    if (sizesFile == NULL) {
        perror("Error opening output sizes file");
        free(inputArray);
        close(fdInput);
        close(fdCompressed);
        close(fdDecompressed);
        return 1;
    }
    fprintf(sizesFile, "Original File Size: %ld bytes\n", inputFileSize);
    fprintf(sizesFile, "Compressed File Size: %ld bytes\n", compressedFileSize);
    fprintf(sizesFile, "Decompressed File Size: %ld bytes\n", decompressedFileSize);

    // Close files and free memory
    fclose(sizesFile);
    free(inputArray);
    close(fdInput);
    close(fdCompressed);
    close(fdDecompressed);

    return 0;
}
