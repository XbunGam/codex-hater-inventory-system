#include <stdio.h>
#include <stdlib.h> // Required for system("clear/cls")

#include "hash.h"
#include "file.h"

// ANSI Color Codes for better UI
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"

static void printHeader(void);
static void printMenu(void);

/*
 * Runs the interactive inventory management program.
 * Input: menu selections and item data read from stdin.
 * Output: returns 0 after saving data and freeing allocated memory.
 */
int main(void) {
    int choice;
    char searchId[ID_SIZE];
    Item *foundItem;

    /* Initialize memory first, then merge any saved records from disk. */
    initializeHashTable();
    loadFromFile();

    while (1) {
        /* Clear the screen each cycle so menu output remains readable. */
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        printHeader();
        printMenu();
        
        printf(BOLD YELLOW " > Select an option: " RESET);

        /* Reject non-numeric input before dispatching menu actions. */
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf(RED "\n [!] Invalid input. Please enter a number (1-7).\n" RESET);
            printf(" Press Enter to continue...");
            getchar();
            continue;
        }
        clearInputBuffer();

        printf("\n");

        switch (choice) {
            case 1:
                printf(CYAN "--- Adding New Inventory ---\n" RESET);
                addItem();
                break;
            case 2:
                printf(RED "--- Remove Item ---\n" RESET);
                removeItem();
                break;
            case 3:
                printf(BLUE "--- Search Database ---\n" RESET);
                printf("Enter Item ID: ");
                readLine(searchId, ID_SIZE);
                foundItem = searchItem(searchId);
                if (foundItem == NULL) {
                    printf(RED "Status: Item '%s' not found.\n" RESET, searchId);
                } else {
                    printf(GREEN "Status: Match located.\n\n" RESET);
                    displayItem(foundItem);
                }
                break;
            case 4:
                updateItem();
                break;
            case 5:
                printf(CYAN "--- Raw Hash Table View ---\n" RESET);
                displayHashTable();
                break;
            case 6:
                printf(CYAN "--- Price-Sorted Inventory (BST) ---\n" RESET);
                displaySortedByPrice();
                break;
            case 7:
                printf(YELLOW " Saving changes and exiting...\n" RESET);
                /* Persist before cleanup because freeHashTable releases all items. */
                saveToFile();
                freeHashTable();
                printf(GREEN " System shutdown clean. Goodbye!\n" RESET);
                return 0;
            default:
                printf(RED " [!] '%d' is not a valid menu choice.\n" RESET, choice);
                break;
        }

        /* Pause so users can read command output before the next screen refresh. */
        printf(BOLD "\nPress Enter to return to menu..." RESET);
        getchar();
    }
}

/*
 * Prints the application title banner.
 * Input: none.
 * Output: none; formatted header text is written to stdout.
 */
static void printHeader(void) {
    printf(CYAN "==========================================\n");
    printf(BOLD "       INVENTORY MANAGEMENT SYSTEM        \n");
    printf(RESET CYAN "==========================================\n" RESET);
}

/*
 * Prints all available menu commands.
 * Input: none.
 * Output: none; formatted menu text is written to stdout.
 */
static void printMenu(void) {
    printf(BOLD " DASHBOARD\n" RESET);
    printf("  " BLUE "1." RESET " Add New Item          " BLUE "5." RESET " View Hash Table\n");
    printf("  " BLUE "2." RESET " Remove Item           " BLUE "6." RESET " View Sorted (Price)\n");
    printf("  " BLUE "3." RESET " Search ID             " BLUE "7." RESET " Exit System\n");
    printf("  " BLUE "4." RESET " Update Details\n");
    printf("------------------------------------------\n");
}
