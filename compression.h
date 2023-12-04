#ifndef HUFFMAN_H
#define HUFFMAN_H

typedef struct Node {
    char data;
    int frequency;
    struct Node* left;
    struct Node* right;
} Node;

Node* createNode(char data, int frequency);
Node* buildHuffmanTree(char data[], int frequency[], int size);
void generateHuffmanCodes(Node* root, int arr[], int top);
void compressFile(char fileName[]);
void decompressFile(char compressedFileName[]);

#endif  // HUFFMAN_H
