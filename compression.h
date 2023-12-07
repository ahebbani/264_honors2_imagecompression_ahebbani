
typedef struct Node {
    char character;
    int freq;
    struct Node* l;
    struct Node* r;
} Node;

// Structure for min heap
struct Min_Heap {
    int size;
    struct Node** array;
};

typedef struct Tree {
    char g;
    int len;
    int dec;
    struct Tree* f;
    struct Tree* r;
} Tree;

// Structure to store codes in compressed file
typedef struct code {
    char k;
    int l;
    int code_arr[16];
    struct code* p;
} code;

void Heapify(struct Min_Heap* Min_Heap, int i);
int isSizeOne(struct Min_Heap* Min_Heap);
void insertIntoMin_Heap(struct Min_Heap* Min_Heap, struct Node* node);
struct Node* extractMinFromMin_Heap(struct Min_Heap* Min_Heap);
int isLeaf(struct Node* root);
int convertBinaryToDecimal(int binary[], int len);
struct Min_Heap* createAndBuildMin_Heap(char arr[], int freq[], int unique_size);
Node* buildHuffmanTree(char arr[], int freq[], int unique_size, struct Min_Heap* Min_Heap);
void printCodesIntoFile(int fd2, struct Node* root, int t[], int top);
void compressFile(int fd1, int fd2, unsigned char a);
void ExtractCodesFromFile(int fd1, Tree *t);
void ReBuildHuffmanTree(int fd1, int size);
void decompressFile(int fd1, int fd2, int f, Tree *tree);