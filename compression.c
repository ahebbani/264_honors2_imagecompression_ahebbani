#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compression.h"

// Helper function to create a new node
Node* createNode(char data, int frequency) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->frequency = frequency;
    newNode->left = newNode->right = NULL;
    return newNode;
}


// Helper function to compare frequencies for sorting nodes
int compareNodes(const void* a, const void* b) {
    return ((Node*)a)->frequency - ((Node*)b)->frequency;
}

// Function to build the Huffman tree
Node* buildHuffmanTree(char data[], int frequency[], int size) {
    Node** nodes = (Node**)malloc(sizeof(Node*) * size);
    for (int i = 0; i < size; ++i) {
        nodes[i] = createNode(data[i], frequency[i]);
    }

    while (size > 1) {
        qsort(nodes, size, sizeof(Node*), compareNodes);

        Node* left = nodes[0];
        Node* right = nodes[1];

        Node* internalNode = createNode('\0', left->frequency + right->frequency);
        internalNode->left = left;
        internalNode->right = right;

        nodes[0] = internalNode;
        nodes[1] = nodes[size - 1];
        size--;
    }

    Node* root = nodes[0];
    free(nodes);

    return root;
}

// Helper function to perform DFS and generate Huffman codes
void generateHuffmanCodesHelper(Node* root, int arr[], int top, char codeTable[][100]) {
    if (root->left) {
        arr[top] = 0;
        generateHuffmanCodesHelper(root->left, arr, top + 1, codeTable);
    }

    if (root->right) {
        arr[top] = 1;
        generateHuffmanCodesHelper(root->right, arr, top + 1, codeTable);
    }

    if (!root->left && !root->right) {
        codeTable[(unsigned char)root->data][top] = '\0';
    }
}

// Function to perform Huffman coding and generate codes for each character
void generateHuffmanCodes(Node* root, int arr[], int top) {
    char codeTable[256][100];
    generateHuffmanCodesHelper(root, arr, top, codeTable);

    printf("Huffman Codes:\n");
    for (int i = 0; i < 256; ++i) {
        if (codeTable[i][0] != '\0') {
            printf("%c: %s\n", (char)i, codeTable[i]);
        }
    }
}


// Function to compress a text file using Huffman coding
void compressFile(char fileName[]) {
    // Read the input text file and count character frequencies
    FILE* inputFile = fopen(fileName, "r");
    if (!inputFile) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    int frequency[256] = {0}; // Assuming ASCII characters
    int totalCharacters = 0;
    int c;

    while ((c = fgetc(inputFile)) != EOF) {
        frequency[c]++;
        totalCharacters++;
    }

    fclose(inputFile);

    // Build the Huffman tree
    char data[256];
    for (int i = 0; i < 256; ++i) {
        data[i] = (char)i;
    }

    Node* root = buildHuffmanTree(data, frequency, 256);

    // Generate Huffman codes
    int arr[100];
    generateHuffmanCodes(root, arr, 0);

    // Create a mapping from characters to their Huffman codes
    char codeTable[256][100];
    generateHuffmanCodesHelper(root, arr, 0, codeTable);

    // Compress the input text file and write the compressed data to an output file
    FILE* outputFile = fopen("compressed.bin", "wb");
    if (!outputFile) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }

    // Write the total number of characters in the original file to the compressed file
    fwrite(&totalCharacters, sizeof(int), 1, outputFile);

    // Rewind the input file to start compressing from the beginning
    fseek(inputFile, 0, SEEK_SET);

    // Write the Huffman codes for each character to the compressed file
    for (int i = 0; i < 256; ++i) {
        if (codeTable[i][0] != '\0') {
            fwrite(&i, sizeof(char), 1, outputFile); // Write the character
            fwrite(codeTable[i], sizeof(char), strlen(codeTable[i]), outputFile); // Write the Huffman code
        }
    }

    // Write the actual compressed data to the output file
    while ((c = fgetc(inputFile)) != EOF) {
        fwrite(codeTable[c], sizeof(char), strlen(codeTable[c]), outputFile);
    }

    fclose(inputFile);
    fclose(outputFile);

    printf("File compressed successfully.\n");
}

// Function to decompress a text file using Huffman coding
void decompressFile(char compressedFileName[]) {
    // Read the compressed file
    FILE* compressedFile = fopen(compressedFileName, "rb");
    if (!compressedFile) {
        perror("Error opening compressed file");
        exit(EXIT_FAILURE);
    }

    // Read the total number of characters in the original file
    int totalCharacters;
    fread(&totalCharacters, sizeof(int), 1, compressedFile);

    // Build the Huffman tree using the stored Huffman codes
    Node* root = NULL;
    while (1) {
        char character;
        fread(&character, sizeof(char), 1, compressedFile);
        if (feof(compressedFile)) {
            break;
        }

        char code[100];
        fread(code, sizeof(char), 100, compressedFile);

        if (!root) {
            root = createNode('\0', 0);
        }

        Node* current = root;
        for (int i = 0; code[i] != '\0'; ++i) {
            if (code[i] == '0') {
                if (!current->left) {
                    current->left = createNode('\0', 0);
                }
                current = current->left;
            } else if (code[i] == '1') {
                if (!current->right) {
                    current->right = createNode('\0', 0);
                }
                current = current->right;
            }
        }
        current->data = character;
    }

    // Decompress the data and write it to an output file
    FILE* outputFile = fopen("decompressed.txt", "w");
    if (!outputFile) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }

    // Read the compressed data and traverse the Huffman tree to decode
    Node* current = root;
    for (int i = 0; i < totalCharacters; ++i) {
        char bit;
        fread(&bit, sizeof(char), 1, compressedFile);

        if (bit == '0') {
            current = current->left;
        } else if (bit == '1') {
            current = current->right;
        }

        if (!current->left && !current->right) {
            fprintf(outputFile, "%c", current->data);
            current = root; // Reset to the root for the next character
        }
    }

    fclose(compressedFile);
    fclose(outputFile);

    printf("File decompressed successfully.\n");
}
