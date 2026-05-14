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

/*
 * Calculates the hash table index for an item ID.
 * Input: id - null-terminated item identifier string.
 * Output: an index in the range [0, TABLE_SIZE - 1].
 */
unsigned int hashFunction(const char *id) {
    unsigned int hash = 0;

    while (*id != '\0') {
        /* Multiplying by 31 spreads similar IDs across buckets with low cost. */
        hash = (hash * 31u) + (unsigned char)(*id);
        id++;
    }

    return hash % TABLE_SIZE;
}

/*
 * Initializes every hash table bucket before inventory operations begin.
 * Input: none.
 * Output: none; global hashTable buckets are reset to NULL.
 */
void initializeHashTable(void) {
    int i;

    for (i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }
}

/*
 * Allocates and initializes a new inventory item.
 * Inputs: id, name, quantity, and price values for the new item.
 * Output: pointer to the allocated Item, or NULL if allocation fails.
 */
Item *createItem(const char *id, const char *name, int quantity, float price) {
    Item *newItem = (Item *)malloc(sizeof(Item));

    if (newItem == NULL) {
        printf("Memory allocation failed. Item was not created.\n");
        return NULL;
    }

    /* Copy bounded strings so oversized user/file input cannot overflow fields. */
    strncpy(newItem->id, id, ID_SIZE - 1);
    newItem->id[ID_SIZE - 1] = '\0';
    strncpy(newItem->name, name, NAME_SIZE - 1);
    newItem->name[NAME_SIZE - 1] = '\0';
    newItem->quantity = quantity;
    newItem->price = price;
    newItem->next = NULL;

    return newItem;
}

/*
 * Searches for an inventory item by ID.
 * Input: id - item identifier to locate.
 * Output: pointer to the matching Item, or NULL when not found.
 */
Item *searchItem(const char *id) {
    unsigned int index = hashFunction(id);
    Item *current = hashTable[index];

    /* Only items in the hashed bucket can match; chaining resolves collisions. */
    while (current != NULL) {
        if (strcmp(current->id, id) == 0) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

/*
 * Prompts the user for item details and inserts the new item into the table.
 * Input: item data read from stdin.
 * Output: none; hashTable is updated when validation and allocation succeed.
 */
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

    /* IDs are unique keys, so duplicates must be rejected before allocation. */
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
    /* Insert at the bucket head for O(1) insertion without traversing the chain. */
    newItem->next = hashTable[index];
    hashTable[index] = newItem;

    printf("Item added successfully at hash index %u.\n", index);
}

/*
 * Removes an item selected by ID from the hash table.
 * Input: item ID read from stdin.
 * Output: none; matching node is unlinked and freed when found.
 */
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
            /* Removing the head bucket entry requires updating the table itself. */
            if (previous == NULL) {
                hashTable[index] = current->next;
            } else {
                previous->next = current->next;
            }

            /* The item owns its allocated node memory, so free it after unlinking. */
            free(current);
            printf("Item removed successfully.\n");
            return;
        }

        previous = current;
        current = current->next;
    }

    printf("Item not found.\n");
}

/*
 * Updates the stored fields for an existing item.
 * Input: item ID and replacement values read from stdin.
 * Output: none; matching Item is modified in place.
 */
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

/*
 * Prints a single inventory item.
 * Input: item - pointer to the item to display.
 * Output: none; formatted item data is written to stdout.
 */
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

/*
 * Displays the full hash table, including collision chains in each bucket.
 * Input: none.
 * Output: none; inventory layout is written to stdout.
 */
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

        /* Walk each bucket's linked list to show collisions in insertion order. */
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

/*
 * Displays inventory ordered by price from lowest to highest.
 * Input: none.
 * Output: none; a temporary BST is built, traversed, and released.
 */
void displaySortedByPrice(void) {
    BSTNode *root = NULL;

    /* The hash table optimizes lookup, so build a short-lived BST for sorting. */
    buildBSTFromHashTable(&root);

    if (root == NULL) {
        printf("No items in inventory.\n");
        return;
    }

    printf("\n--- Inventory Sorted by Price (Lowest to Highest) ---\n");
    inOrderTraversal(root);
    freeBST(root);
}

/*
 * Frees every dynamically allocated item stored in the hash table.
 * Input: none.
 * Output: none; all buckets are reset to NULL.
 */
void freeHashTable(void) {
    int i;
    Item *current;
    Item *next;

    for (i = 0; i < TABLE_SIZE; i++) {
        current = hashTable[i];

        while (current != NULL) {
            /* Save the next pointer before freeing the current node. */
            next = current->next;
            free(current);
            current = next;
        }

        hashTable[i] = NULL;
    }
}

/*
 * Reads one line of text safely from stdin.
 * Inputs: buffer - destination array, size - destination capacity.
 * Output: none; buffer receives a null-terminated string without the newline.
 */
void readLine(char *buffer, int size) {
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }

    buffer[strcspn(buffer, "\n")] = '\0';
}

/*
 * Discards unread characters left in stdin after formatted input.
 * Input: none.
 * Output: none; stdin is advanced to the end of the current line.
 */
void clearInputBuffer(void) {
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF) {
        /* Discard remaining characters on the current input line. */
    }
}

/*
 * Repeatedly prompts for an integer until valid input is received.
 * Input: prompt - message shown before each read attempt.
 * Output: parsed integer value.
 */
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

/*
 * Repeatedly prompts for a floating-point number until valid input is received.
 * Input: prompt - message shown before each read attempt.
 * Output: parsed float value.
 */
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

/*
 * Allocates a BST node that references an existing inventory item.
 * Input: item - inventory item to associate with the BST node.
 * Output: pointer to the allocated BSTNode, or NULL on allocation failure.
 */
static BSTNode *createBSTNode(Item *item) {
    BSTNode *node = (BSTNode *)malloc(sizeof(BSTNode));

    if (node == NULL) {
        printf("Memory allocation failed while building BST.\n");
        return NULL;
    }

    node->item = item;
    node->left = NULL;
    node->right = NULL;

    /* The BST does not own Item memory; it only references hash table items. */
    return node;
}

/*
 * Inserts an item into the temporary BST sorted by price, then ID.
 * Inputs: root - current subtree root, item - inventory item to insert.
 * Output: root pointer for the updated subtree.
 */
static BSTNode *insertBST(BSTNode *root, Item *item) {
    if (root == NULL) {
        return createBSTNode(item);
    }

    /* Price is the primary sort key; ID breaks ties for deterministic ordering. */
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

/*
 * Builds a temporary BST containing all items currently in the hash table.
 * Input: root - address of the BST root pointer to populate.
 * Output: none; *root is updated as items are inserted.
 */
static void buildBSTFromHashTable(BSTNode **root) {
    int i;
    Item *current;

    for (i = 0; i < TABLE_SIZE; i++) {
        current = hashTable[i];

        while (current != NULL) {
            /* Insert each chained item so the final traversal is price-sorted. */
            *root = insertBST(*root, current);
            current = current->next;
        }
    }
}

/*
 * Performs an in-order traversal of the BST.
 * Input: root - subtree root to traverse.
 * Output: none; items are printed in ascending sort order.
 */
static void inOrderTraversal(const BSTNode *root) {
    if (root == NULL) {
        return;
    }

    /* Left-root-right traversal returns sorted order for a BST. */
    inOrderTraversal(root->left);
    displayItem(root->item);
    inOrderTraversal(root->right);
}

/*
 * Frees all temporary BST nodes created for sorted display.
 * Input: root - subtree root to release.
 * Output: none; only BST nodes are freed, not the referenced Items.
 */
static void freeBST(BSTNode *root) {
    if (root == NULL) {
        return;
    }

    freeBST(root->left);
    freeBST(root->right);
    free(root);
}
