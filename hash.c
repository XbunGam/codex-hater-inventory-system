#include "hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct BSTNode {
    Item *item;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

Item *hashTable[TABLE_SIZE];

static int readInt(const char *prompt);
static float readFloat(const char *prompt);
static BSTNode *createBSTNode(Item *item);
static BSTNode *insertBST(BSTNode *root, Item *item);
static void buildBSTFromHashTable(BSTNode **root);
static void inOrderTraversal(const BSTNode *root);
static void freeBST(BSTNode *root);

unsigned int hashFunction(const char *id) {
    unsigned int hash = 0;

    while (*id != '\0') {
        //67 hash
        hash = (hash * 67u) + (unsigned char)(*id);
        id++;
    }

    return hash % TABLE_SIZE;
}

void initializeHashTable(void) {
    int i;

    for (i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }
}

Item *createItem(const char *id, const char *name, int quantity, float price) {
    Item *newItem = (Item *)malloc(sizeof(Item));

    if (newItem == NULL) {
        printf("Memory allocation failed. Item was not created.\n");
        return NULL;
    }

    strncpy(newItem->id, id, ID_SIZE - 1);
    newItem->id[ID_SIZE - 1] = '\0';
    strncpy(newItem->name, name, NAME_SIZE - 1);
    newItem->name[NAME_SIZE - 1] = '\0';
    newItem->quantity = quantity;
    newItem->price = price;
    newItem->next = NULL;

    return newItem;
}

Item *searchItem(const char *id) {
    unsigned int index = hashFunction(id);
    Item *current = hashTable[index];

    while (current != NULL) {
        if (strcmp(current->id, id) == 0) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

void addItem(void) {
    char id[ID_SIZE];
    char name[NAME_SIZE];
    int quantity;
    float price;
    unsigned int index;
    Item *newItem;

    printf("Enter Item ID: ");
    readLine(id, ID_SIZE);

    if (strlen(id) == 0) {
        printf("Item ID cannot be empty.\n");
        return;
    }

    if (searchItem(id) != NULL) {
        printf("Item ID already exists. Use update instead.\n");
        return;
    }

    printf("Enter Item Name: ");
    readLine(name, NAME_SIZE);

    quantity = readInt("Enter Quantity: ");
    price = readFloat("Enter Price: ");

    newItem = createItem(id, name, quantity, price);
    if (newItem == NULL) {
        return;
    }

    index = hashFunction(id);
    newItem->next = hashTable[index];
    hashTable[index] = newItem;

    printf("Item added successfully at hash index %u.\n", index);
}

void removeItem(void) {
    char id[ID_SIZE];
    unsigned int index;
    Item *current;
    Item *previous = NULL;

    printf("Enter Item ID to remove: ");
    readLine(id, ID_SIZE);

    index = hashFunction(id);
    current = hashTable[index];

    while (current != NULL) {
        if (strcmp(current->id, id) == 0) {
            if (previous == NULL) {
                hashTable[index] = current->next;
            } else {
                previous->next = current->next;
            }

            free(current);
            printf("Item removed successfully.\n");
            return;
        }

        previous = current;
        current = current->next;
    }

    printf("Item not found.\n");
}

void updateItem(void) {
    char id[ID_SIZE];
    char name[NAME_SIZE];
    Item *item;

    printf("Enter Item ID to update: ");
    readLine(id, ID_SIZE);

    item = searchItem(id);
    if (item == NULL) {
        printf("Item not found.\n");
        return;
    }

    printf("Current item details:\n");
    displayItem(item);

    printf("Enter new Item Name: ");
    readLine(name, NAME_SIZE);
    strncpy(item->name, name, NAME_SIZE - 1);
    item->name[NAME_SIZE - 1] = '\0';

    item->quantity = readInt("Enter new Quantity: ");
    item->price = readFloat("Enter new Price: ");

    printf("Item updated successfully.\n");
}

void displayItem(const Item *item) {
    if (item == NULL) {
        return;
    }

    printf("ID: %s | Name: %s | Quantity: %d | Price: %.2f\n",
           item->id,
           item->name,
           item->quantity,
           item->price);
}

void displayHashTable(void) {
    int i;
    Item *current;
    int isEmpty = 1;

    printf("\n--- Hash Table Inventory ---\n");
    for (i = 0; i < TABLE_SIZE; i++) {
        printf("[%d]: ", i);
        current = hashTable[i];

        if (current == NULL) {
            printf("Empty");
        }

        while (current != NULL) {
            isEmpty = 0;
            printf("(%s, %s, Qty: %d, Price: %.2f)",
                   current->id,
                   current->name,
                   current->quantity,
                   current->price);

            if (current->next != NULL) {
                printf(" -> ");
            }
            current = current->next;
        }

        printf("\n");
    }

    if (isEmpty) {
        printf("No items in inventory.\n");
    }
}

void displaySortedByPrice(void) {
    BSTNode *root = NULL;

    buildBSTFromHashTable(&root);

    if (root == NULL) {
        printf("No items in inventory.\n");
        return;
    }

    printf("\n--- Inventory Sorted by Price (Lowest to Highest) ---\n");
    inOrderTraversal(root);
    freeBST(root);
}

void freeHashTable(void) {
    int i;
    Item *current;
    Item *next;

    for (i = 0; i < TABLE_SIZE; i++) {
        current = hashTable[i];

        while (current != NULL) {
            next = current->next;
            free(current);
            current = next;
        }

        hashTable[i] = NULL;
    }
}

void readLine(char *buffer, int size) {
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }

    buffer[strcspn(buffer, "\n")] = '\0';
}

void clearInputBuffer(void) {
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF) {
        /* Discard remaining characters on the current input line. */
    }
}

static int readInt(const char *prompt) {
    int value;
    int result;

    while (1) {
        printf("%s", prompt);
        result = scanf("%d", &value);
        clearInputBuffer();

        if (result == 1) {
            return value;
        }

        printf("Invalid input. Please enter an integer.\n");
    }
}

static float readFloat(const char *prompt) {
    float value;
    int result;

    while (1) {
        printf("%s", prompt);
        result = scanf("%f", &value);
        clearInputBuffer();

        if (result == 1) {
            return value;
        }

        printf("Invalid input. Please enter a number.\n");
    }
}

static BSTNode *createBSTNode(Item *item) {
    BSTNode *node = (BSTNode *)malloc(sizeof(BSTNode));

    if (node == NULL) {
        printf("Memory allocation failed while building BST.\n");
        return NULL;
    }

    node->item = item;
    node->left = NULL;
    node->right = NULL;

    return node;
}

static BSTNode *insertBST(BSTNode *root, Item *item) {
    if (root == NULL) {
        return createBSTNode(item);
    }

    if (item->price < root->item->price) {
        root->left = insertBST(root->left, item);
    } else if (item->price > root->item->price) {
        root->right = insertBST(root->right, item);
    } else if (strcmp(item->id, root->item->id) < 0) {
        root->left = insertBST(root->left, item);
    } else {
        root->right = insertBST(root->right, item);
    }

    return root;
}

static void buildBSTFromHashTable(BSTNode **root) {
    int i;
    Item *current;

    for (i = 0; i < TABLE_SIZE; i++) {
        current = hashTable[i];

        while (current != NULL) {
            *root = insertBST(*root, current);
            current = current->next;
        }
    }
}

static void inOrderTraversal(const BSTNode *root) {
    if (root == NULL) {
        return;
    }

    inOrderTraversal(root->left);
    displayItem(root->item);
    inOrderTraversal(root->right);
}

static void freeBST(BSTNode *root) {
    if (root == NULL) {
        return;
    }

    freeBST(root->left);
    freeBST(root->right);
    free(root);
}
