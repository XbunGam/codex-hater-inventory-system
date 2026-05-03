#include <stdio.h>

#include "hash.h"
#include "file.h"

static void printMenu(void);

int main(void) {
    int choice;
    char searchId[ID_SIZE];
    Item *foundItem;

    initializeHashTable();
    loadFromFile();

    /*
     * Main command-line interface loop.
     * The loop runs until the user selects option 7, then memory is freed
     * before the program returns.
     */
    while (1) {
        printMenu();
        printf("Enter your choice: ");

        /*
         * scanf reads the numeric menu choice. The input buffer is cleared
         * afterward so later string prompts do not accidentally read leftovers.
         */
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input. Please enter a number from 1 to 7.\n");
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                addItem();
                break;
            case 2:
                removeItem();
                break;
            case 3:
                printf("Enter Item ID to search: ");
                readLine(searchId, ID_SIZE);
                foundItem = searchItem(searchId);
                if (foundItem == NULL) {
                    printf("Item not found.\n");
                } else {
                    printf("Item found:\n");
                    displayItem(foundItem);
                }
                break;
            case 4:
                updateItem();
                break;
            case 5:
                displayHashTable();
                break;
            case 6:
                displaySortedByPrice();
                break;
            case 7:
                printf("Exiting program. Freeing memory...\n");
                saveToFile();
                freeHashTable();
                return 0;
            default:
                printf("Invalid choice. Please select a valid menu option.\n");
                break;
        }
    }
}

static void printMenu(void) {
    printf("\n========== Inventory System ==========\n");
    printf("1. Add New Item\n");
    printf("2. Remove Item\n");
    printf("3. Search Item\n");
    printf("4. Update Item Details\n");
    printf("5. View Full Inventory Table (Hash Table view)\n");
    printf("6. View Inventory Sorted by Price (Using BST logic)\n");
    printf("7. Exit Program\n");
}
