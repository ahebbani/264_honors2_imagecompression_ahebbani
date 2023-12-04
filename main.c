#include <stdio.h>
#include <stdlib.h>
#include "compression.h"

int main() {
    // Example usage
    char fileName[] = "input.txt";
    char compressedFileName[] = "compressed.bin";

    // Compress the input text file
    compressFile(fileName);

    // Decompress the compressed file
    decompressFile(compressedFileName);

    return 0;
}