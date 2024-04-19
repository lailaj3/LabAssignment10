#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ALPHABET_SIZE 26
// Trie structure
struct TrieNode
{
    struct TrieNode* children[ALPHABET_SIZE];
    int count;
};
struct Trie {
    struct TrieNode* root;
};
struct TrieNode* createTrieNode() {
    struct TrieNode* newNode = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        newNode->children[i] = NULL;
    }
    newNode->count = 0;
    return newNode;
}
// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    struct TrieNode* curr = pTrie->root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (curr->children[index] == NULL) {
            curr->children[index] = createTrieNode();
        }
        curr = curr->children[index];
    }
    curr->count++;
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    struct TrieNode* curr = pTrie->root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (curr->children[index] == NULL) {
            return 0;
        }
        curr = curr->children[index];
    }
    return curr->count;
}
struct TrieNode* deallocateTrieNode(struct TrieNode* pNode) {
    if (pNode == NULL) {
        return NULL;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        pNode->children[i] = deallocateTrieNode(pNode->children[i]);
    }
    free(pNode);
    return NULL;
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    pTrie->root = deallocateTrieNode(pTrie->root);
    free(pTrie);
    return NULL;
}

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie* pTrie = (struct Trie*)malloc(sizeof(struct Trie));
    pTrie->root = createTrieNode();
    return pTrie;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    FILE* fp;
    int numWords = 0;
    char word[256];

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
        exit(1);
    }

    fscanf(fp, "%d", &numWords); // Read the number of words from the file

    for (int i = 0; i < numWords; ++i) {
        fscanf(fp, "%s", word);
        pInWords[i] = strdup(word);
    }

    fclose(fp);
    return numWords;
}

int main(void)
{
    char *inWords[256];

    //read the number of the words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i=0;i<numWords;++i)
    {
        printf("%s\n",inWords[i]);
    }

    struct Trie *pTrie = createTrie();
    for (int i=0;i<numWords;i++)
    {
        insert(pTrie, inWords[i]);
    }
    // parse line by line, and insert each word to the trie data structure
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i=0;i<5;i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }
    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
        printf("There is an error in this program\n");

    for (int i = 0; i < numWords; ++i)
        free(inWords[i]);
    return 0;
}
