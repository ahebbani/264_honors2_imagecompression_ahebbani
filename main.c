#include "compression.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <inputFileName>\n", argv[0]);
        return 1;
    }

    const char *inputFileName = argv[1];
    const char *compressedFileName = "compressed.huff";
    const char *decompressedFileName = "decompressed.txt";

    // Compress the input file
    compressFile(inputFileName, compressedFileName);

    // Decompress the compressed file
    decompressFile(compressedFileName, decompressedFileName);

    return 0;
}
