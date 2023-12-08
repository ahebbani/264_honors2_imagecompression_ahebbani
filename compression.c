#include <stdlib.h>
#include <string.h>
#include "compression.h"

void freeTree(struct MinHeapNode *root) {
    if (root) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

// Structure to represent a Huffman tree node
struct MinHeapNode *newNode(char data, unsigned freq) {
    struct MinHeapNode *node = (struct MinHeapNode *)malloc(sizeof(struct MinHeapNode));
    node->left = node->right = NULL;
    node->data = data;
    node->freq = freq;
    return node;
}

struct MinHeap *createMinHeap(unsigned capacity) {
    struct MinHeap *minHeap = (struct MinHeap *)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode **)malloc(capacity * sizeof(struct MinHeapNode *));
    return minHeap;
}

void swapMinHeapNode(struct MinHeapNode **a, struct MinHeapNode **b) {
    struct MinHeapNode *t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(struct MinHeap *minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq) {
        smallest = left;
    }

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq) {
        smallest = right;
    }

    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isSizeOne(struct MinHeap *minHeap) {
    return (minHeap->size == 1);
}

struct MinHeapNode *extractMin(struct MinHeap *minHeap) {
    struct MinHeapNode *temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

void insertMinHeap(struct MinHeap *minHeap, struct MinHeapNode *node) {
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && node->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = node;
}

struct MinHeap *buildMinHeap(char data[], int freq[], int size) {
    struct MinHeap *minHeap = createMinHeap(size);
    for (int i = 0; i < size; i++) // Changed ++i to i++
        minHeap->array[i] = newNode(data[i], freq[i]);
    minHeap->size = size;
    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
    return minHeap;
}

struct MinHeapNode *buildHuffmanTree(char data[], int freq[], int size) {
    struct MinHeapNode *left, *right, *top;
    struct MinHeap *minHeap = buildMinHeap(data, freq, size);
    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);
        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }
    return extractMin(minHeap);
}

void storeCodes(struct MinHeapNode *root, int arr[], int top, int codes[][256]) {
    if (root->left) {
        arr[top] = 0;
        storeCodes(root->left, arr, top + 1, codes);
    }
    if (root->right) {
        arr[top] = 1;
        storeCodes(root->right, arr, top + 1, codes);
    }
    if (!root->left && !root->right) {
        for (int i = 0; i < top; ++i) {
            codes[(unsigned char)root->data][i] = arr[i];
        }
    }
}

void writeTreeToFile(struct MinHeapNode *root, FILE *outputFile) {
    if (root) {
        if (!root->left && !root->right) {
            fputc('1', outputFile);
            fputc(root->data, outputFile);
        } else {
            fputc('0', outputFile);
            writeTreeToFile(root->left, outputFile);
            writeTreeToFile(root->right, outputFile);
        }
    }
}

void writeBit(FILE *outputFile, int bit) {
    static int currentByte = 0;
    static unsigned char currentChar = 0;

    currentChar = (currentChar << 1) | bit;
    ++currentByte;

    if (currentByte == 8) {
        fputc(currentChar, outputFile);
        currentByte = 0;
        currentChar = 0;
    }
}

void writeCode(FILE *outputFile, int code[], int size) {
    for (int i = 0; i < size; ++i) {
        writeBit(outputFile, code[i]);
    }
}

void compressFile(const char *inputFileName, const char *outputFileName) {
    FILE *inputFile = fopen(inputFileName, "r");
    FILE *outputFile = fopen(outputFileName, "wb");

    if (!inputFile || !outputFile) {
        fprintf(stderr, "Error opening files\n");
        return;
    }

    // Get file size
    fseek(inputFile, 0, SEEK_END);
    long fileSize = ftell(inputFile);
    fseek(inputFile, 0, SEEK_SET);

    // Print input file size
    printf("Input File Size: %ld bytes\n", fileSize);

    // Initialize frequency array
    int freq[256] = {0};

    // Count frequencies of characters in the file
    int c;
    while ((c = fgetc(inputFile)) != EOF) {
        ++freq[(unsigned char)c];
    }

    // Build Huffman tree
    struct MinHeapNode *root = buildHuffmanTree(NULL, freq, 256);

    // Store Huffman codes in an array
    int arr[256];
    int codes[256][256];
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            codes[i][j] = 0;
        }
    }
    storeCodes(root, arr, 0, codes);

    // Write Huffman tree to the output file
    writeTreeToFile(root, outputFile);

    // Reset file pointer to the beginning
    fseek(inputFile, 0, SEEK_SET);

    // Write compressed data to the output file
    while ((c = fgetc(inputFile)) != EOF) {
        writeCode(outputFile, codes[(unsigned char)c], strlen((char *)codes[(unsigned char)c]));
    }

    fclose(inputFile);
    fclose(outputFile);

    // Free allocated memory
    freeTree(root);
    fclose(inputFile);
    fclose(outputFile);
}

struct MinHeapNode *buildHuffmanTreeFromFile(FILE *inputFile) {
    int bit = fgetc(inputFile);
    if (bit == '1') {
        int character = fgetc(inputFile);
        return newNode(character, 0);
    } else {
        struct MinHeapNode *left = buildHuffmanTreeFromFile(inputFile);
        struct MinHeapNode *right = buildHuffmanTreeFromFile(inputFile);
        return newNode('$', left->freq + right->freq);
    }
}

void decompressFile(const char *inputFileName, const char *outputFileName) {
    FILE *inputFile = fopen(inputFileName, "rb");
    FILE *outputFile = fopen(outputFileName, "w");

    // Get compressed file size
    fseek(inputFile, 0, SEEK_END);
    long compressedSize = ftell(inputFile);
    fseek(inputFile, 0, SEEK_SET);

    // Print compressed file size
    printf("Compressed File Size: %ld bytes\n", compressedSize);

    // Build Huffman tree from the input file
    struct MinHeapNode *root = buildHuffmanTreeFromFile(inputFile);

    // Decompress and write to the output file
    struct MinHeapNode *currentNode = root;
    int bit;
    while ((bit = fgetc(inputFile)) != EOF) {
        if (bit == '0') {
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right;
        }

        if (!currentNode->left && !currentNode->right) {
            fputc(currentNode->data, outputFile);
            currentNode = root;
            compressedSize--;
        }
    }

    fclose(inputFile);
    fclose(outputFile);
}