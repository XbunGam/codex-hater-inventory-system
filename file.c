#include "file.h"

#include <stdio.h>
#include <string.h>

#include "hash.h"

#define DATA_FILE "data.csv"
#define LINE_SIZE 256

static void insertLoadedItem(const char *id, const char *name, int quantity, float price);

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
        line[strcspn(line, "\n")] = '\0';

        if (sscanf(line, "%49[^,],%99[^,],%d,%f", id, name, &quantity, &price) == 4) {
            insertLoadedItem(id, name, quantity, price);
        }
    }

    fclose(file);
}

static void insertLoadedItem(const char *id, const char *name, int quantity, float price) {
    unsigned int index;
    Item *newItem;

    if (strlen(id) == 0 || searchItem(id) != NULL) {
        return;
    }

    newItem = createItem(id, name, quantity, price);
    if (newItem == NULL) {
        return;
    }

    index = hashFunction(id);
    newItem->next = hashTable[index];
    hashTable[index] = newItem;
}
