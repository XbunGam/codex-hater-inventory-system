#ifndef HASH_H
#define HASH_H

#define TABLE_SIZE 20
#define ID_SIZE 50
#define NAME_SIZE 100

/*
 * Inventory System data structures:
 *
 * 1. Hash Table with separate chaining
 *    - Suitable for fast item lookup by Item ID.
 *    - Average-case add, search, update, and delete are O(1).
 *
 * 2. Binary Search Tree (BST)
 *    - Built temporarily from current inventory items to sort by price.
 *    - In-order traversal displays prices from lowest to highest.
 */

typedef struct Item {
    char id[ID_SIZE];
    char name[NAME_SIZE];
    int quantity;
    float price;
    struct Item *next;
} Item;

extern Item *hashTable[TABLE_SIZE];

unsigned int hashFunction(const char *id);
void initializeHashTable(void);
Item *createItem(const char *id, const char *name, int quantity, float price);
Item *searchItem(const char *id);
void addItem(void);
void removeItem(void);
void updateItem(void);
void displayItem(const Item *item);
void displayHashTable(void);
void displaySortedByPrice(void);
void freeHashTable(void);

void readLine(char *buffer, int size);
void clearInputBuffer(void);

#endif
