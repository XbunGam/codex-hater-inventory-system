# InvenSys

A console-based inventory management system written in C for a CPE112 final project that can be run in Terminal. The program stores inventory items by ID, supports common inventory operations, saves data to a CSV file, and can display items sorted by price.

## Features

- Add new inventory items
- Remove items by item ID
- Search for items by item ID
- Update item name, quantity, and price
- Display the raw hash table structure
- Display inventory sorted by price from lowest to highest
- Load inventory records from `data.csv`
- Save inventory records back to `data.csv` before exiting

## Data Structures Used

### Hash Table

The main inventory storage uses a hash table with separate chaining. Each item ID is hashed into one of the table indexes, and collisions are handled with a linked list.

This makes item operations efficient:

- Add: average `O(1)`
- Search: average `O(1)`
- Update: average `O(1)`
- Delete: average `O(1)`

### Binary Search Tree

The program temporarily builds a Binary Search Tree when the user chooses to view items sorted by price. An in-order traversal of the BST displays inventory items from the lowest price to the highest price.

## Project Files

| File | Description |
| --- | --- |
| `main.c` | Main program loop, menu display, and user option handling |
| `hash.h` | Inventory item structure and function declarations |
| `hash.c` | Hash table operations, item management, and BST sorting |
| `file.h` | File handling function declarations |
| `file.c` | CSV load and save logic |
| `data.csv` | Saved inventory data |

## CSV Format

Inventory data is stored in `data.csv` using this format:

```csv
item_id,item_name,quantity,price
```

Example:

```csv
333,Nestle Pure Life,350,7.00
345,Eggs,100,7.00
```

## How to Compile

Using GCC:

```powershell
gcc main.c hash.c file.c -o inventory_system.exe
```

## How to Run

On Windows PowerShell or Terminal:

```powershell
.\inventory_system.exe
```

## Menu Options

When the program starts, it shows a dashboard with these options:

| Option | Action |
| --- | --- |
| 1 | Add New Item |
| 2 | Remove Item |
| 3 | Search ID |
| 4 | Update Details |
| 5 | View Hash Table |
| 6 | View Sorted by Price |
| 7 | Save Files |
| 8 | Exit System |

Choose option `8` to save the inventory and exit safely.

## Notes

- Item IDs must be unique.
- Empty item IDs are not allowed.
- The program automatically loads existing records from `data.csv` when it starts.
- The program saves the current inventory to `data.csv` when exiting through the menu.
- Item names should not contain commas because commas are used as CSV separators.


## Contributions

#### 🔹 Pholawath Channukool 68070503443 — Data Structures & Core & BST (Foundation Layer)
* **Module Ownership:** `hash.h` (100%), Base structural layers of `hash.c`.
* **Responsibilities:**
    * Define core tracking architectures: `Item` and `BSTNode` structs.
    * Implement structural foundations: table initialization (`initHashTable()`) and the primary string hash processing algorithm (`hashFunction()`).
    * Manage node memory lifecycles: entity creation, hash indexing lookups, and table-wide recursive pointer clearing (`freeTable()`).
    * Provide universal input serialization abstractions shared across all files (`readLine()`, `readInt()`, etc.).
* *Note: This layer contains blocking architecture that must be prioritized and completed first.*

#### 🔹 Nassasi Treesuptavee 68070503421 — CRUD Operations & UI (Interaction Layer)
* **Module Ownership:** `main.c` (100%), Interaction/API functions inside `hash.c`.
* **Responsibilities:**
    * Construct the core application loop dashboard utilizing ANSI escape sequences for stylized terminal updates.
    * Build standard inventory ingestion and deletion APIs: `addItem()` and `removeItem()`.
    * Implement safe data mutation pathways for modifying existing system records: `updateItem()`.
    * Develop terminal rendering pipelines for targeted single-item lookups (`displayItem()`) and complete memory index printing (`displayHashTable()`).

#### 🔹 Wasuphon Chaosahnguan 68070503453 — File I/O & Sorting & BST (Persistence & Analytics)
* **Module Ownership:** `file.h` & `file.c` (100%), Sorting utility sub-routing inside `hash.c`.
* **Responsibilities:**
    * Implement persistent storage transformations: serializing memory structures out to disk and parsing `.csv` files (`saveInventory()`, `loadInventory()`).
    * Provide safe structural routing functions utilized heavily during raw text parsing operations (`insertLoadedItem()`).
    * Construct ephemeral Binary Search Trees to handle transient data points for multi-axis price sorting (`buildBSTFromHash()`, `insertBSTNode()`).
    * Execute memory-safe pointer ordering and sorted print outputs before tearing down temporary structures (`inorderBST()`, `freeBST()`).
    

---

## 🔄 Execution Workflow
