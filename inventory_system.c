#include <stdio.h>

#include "file.h"
#include "hash.h"


static void printMenu(void);

/*
 * Runs the console inventory management program.
 * Input: menu choices and item data read from stdin.
 * Output: returns 0 after saving inventory data and freeing memory.
 */
int main(void) {
    int choice;
    char searchId[ID_SIZE];
    Item *foundItem;

    /* Prepare the hash table before loading persisted items into it. */
    initializeHashTable();
    loadFromFile();

    while (1) {
        printMenu();
        printf("Enter your choice: ");

        /* Validate menu input so invalid text does not drive the switch logic. */
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
                /* Save first because cleanup releases all dynamically allocated items. */
                saveToFile();
                freeHashTable();
                return 0;
            default:
                printf("Invalid choice. Please select a valid menu option.\n");
                break;
        }
    }
}
