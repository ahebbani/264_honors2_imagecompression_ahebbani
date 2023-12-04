#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> 
#include "compression.h"

static bool readArray(const char* filename, int** array, int* size) {
    FILE* fptr = fopen(filename, "r");
    if (fptr == NULL) {
        return false;
    }
    int numint = 0;
    int value;
    while (fscanf(fptr, "%d", &value) == 1) {
        numint++;
    }
    if (numint == 0) {
        // no integer to read
        return false;
    }
    *size = numint;
    int* arr = malloc(sizeof(int) * numint);
    fseek(fptr, 0, SEEK_SET);
    int ind = 0;
    while (ind < numint) {
        if (fscanf(fptr, "%d", &arr[ind]) != 1) {
            fprintf(stderr, "SOMETHING WRONG\n");
            free(arr);
            *size = 0;
            return false;
        }
        ind++;
    }
    fclose(fptr);
    *array = arr;
    return true;
}

// Helper function to get the size of a file
long getFileSize(FILE* file) {
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

int main(int argc, char** argv) {
    // Read file
    if (argc < 2) {
        return EXIT_FAILURE;
    }
    int* inArray = NULL;  // Change from char* to int*
    int insize;
    bool rtv;
    rtv = readArray(argv[1], &inArray, &insize);
    if (rtv == false) {
        return EXIT_FAILURE;
    }

    // Example usage
    char compressedFileName[] = "compressed.bin";
    char decompressedFileName[] = "decompressed.txt";

    // Get the size of the original file
    FILE* originalFile = fopen(argv[1], "rb");
    if (!originalFile) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }
    long originalSize = getFileSize(originalFile);
    fclose(originalFile);

    // Compress the input text file
    compressFile(argv[1]);

    // Get the size of the compressed file
    FILE* compressedFile = fopen(compressedFileName, "rb");
    if (!compressedFile) {
        perror("Error opening compressed file");
        return EXIT_FAILURE;
    }
    long compressedSize = getFileSize(compressedFile);
    fclose(compressedFile);

    // Decompress the compressed file
    decompressFile(compressedFileName);

    // Get the size of the decompressed file
    FILE* decompressedFile = fopen(decompressedFileName, "rb");
    if (!decompressedFile) {
        perror("Error opening decompressed file");
        return EXIT_FAILURE;
    }
    long decompressedSize = getFileSize(decompressedFile);
    fclose(decompressedFile);

    // Compare file sizes
    printf("Original File Size: %ld bytes\n", originalSize);
    printf("Compressed File Size: %ld bytes\n", compressedSize);
    printf("Decompressed File Size: %ld bytes\n", decompressedSize);

    return 0;
}
