#include "file.h"

#include <stdio.h>
#include <string.h>

#include "hash.h"

#define DATA_FILE "data.csv"
#define LINE_SIZE 256

static void insertLoadedItem(const char *id, const char *name, int quantity, float price);

/*
 * Saves the current inventory to a CSV file.
 * Input: none; data is read from the global hashTable.
 * Output: none; DATA_FILE is overwritten with the current inventory state.
 */
void saveToFile(void) {
    FILE *file = fopen(DATA_FILE, "w");
    int i;
    Item *current;

    if (file == NULL) {
        printf("Error: Could not open %s for writing.\n", DATA_FILE);
        return;
    }

    for (i = 0; i < TABLE_SIZE; i++) {
        current = hashTable[i];

        /* Persist every item in every collision chain, one CSV record per item. */
        while (current != NULL) {
            fprintf(file, "%s,%s,%d,%.2f\n",
                    current->id,
                    current->name,
                    current->quantity,
                    current->price);
            current = current->next;
        }
    }

    fclose(file);
}

/*
 * Loads inventory records from the CSV data file.
 * Input: none; DATA_FILE is read if it exists.
 * Output: none; valid records are inserted into the global hashTable.
 */
void loadFromFile(void) {
    FILE *file = fopen(DATA_FILE, "r");
    char line[LINE_SIZE];
    char id[ID_SIZE];
    char name[NAME_SIZE];
    int quantity;
    float price;

    if (file == NULL) {
        return;
    }

    while (fgets(line, LINE_SIZE, file) != NULL) {
        /* Strip the newline so parsing works consistently for the final field. */
        line[strcspn(line, "\n")] = '\0';

        /*
         * Parse comma-separated fields with width limits that match Item buffers.
         * Malformed lines are skipped instead of partially loading bad records.
         */
        if (sscanf(line, "%49[^,],%99[^,],%d,%f", id, name, &quantity, &price) == 4) {
            insertLoadedItem(id, name, quantity, price);
        }
    }

    fclose(file);
}

/*
 * Inserts one item loaded from storage into the hash table.
 * Inputs: id, name, quantity, and price parsed from the CSV file.
 * Output: none; duplicate or invalid records are ignored.
 */
static void insertLoadedItem(const char *id, const char *name, int quantity, float price) {
    unsigned int index;
    Item *newItem;

    /* Avoid corrupting the unique-key model when the file contains duplicates. */
    if (strlen(id) == 0 || searchItem(id) != NULL) {
        return;
    }

    newItem = createItem(id, name, quantity, price);
    if (newItem == NULL) {
        return;
    }

    index = hashFunction(id);
    /* Use the same head insertion strategy as interactive item creation. */
    newItem->next = hashTable[index];
    hashTable[index] = newItem;
}
