> [!NOTE]
> I'll be turning this repo into a port of Meta's [`FBVector`](https://github.com/facebook/folly/blob/main/folly/docs/FBVector.md), and I'll also try to add some doxygen comments.

# arraylist

generic `ArrayList` in C

## Installation

```sh
git clone git@github.com:simontran7/arraylist.git
cd arraylist
cp arraylist.h <your source folder>
```

## Usage

### Simple Example

```c
#include "arraylist.h"
#include <stdio.h>

// Generate ArrayList for integers
GENERATE_ARRAYLIST(int_list, int)

int main() {
    // Create a new ArrayList
    ArrayList_int_list *list = ARRAYLIST_CREATE(int_list);
    
    // Add elements
    ARRAYLIST_ADD_LAST(int_list, list, 10);
    ARRAYLIST_ADD_LAST(int_list, list, 20);
    ARRAYLIST_ADD_LAST(int_list, list, 30);
    ARRAYLIST_ADD_FIRST(int_list, list, 5);
    
    // Get list length
    printf("List length: %zu\n", ARRAYLIST_LEN(int_list, list));
    
    // Access elements
    int value;
    if (ARRAYLIST_GET(int_list, list, 0, &value) == SUCCESS_int_list) {
        printf("First element: %d\n", value);
    }
    
    if (ARRAYLIST_GET_LAST(int_list, list, &value) == SUCCESS_int_list) {
        printf("Last element: %d\n", value);
    }
    
    // Print all elements
    printf("All elements: ");
    for (size_t i = 0; i < ARRAYLIST_LEN(int_list, list); i++) {
        if (ARRAYLIST_GET(int_list, list, i, &value) == SUCCESS_int_list) {
            printf("%d ", value);
        }
    }
    printf("\n");
    
    // Remove elements
    if (ARRAYLIST_REMOVE_FIRST(int_list, list, &value) == SUCCESS_int_list) {
        printf("Removed first: %d\n", value);
    }
    
    if (ARRAYLIST_REMOVE_LAST(int_list, list, &value) == SUCCESS_int_list) {
        printf("Removed last: %d\n", value);
    }
    
    // Insert at specific index
    ARRAYLIST_ADD(int_list, list, 1, 15);
    
    // Update element
    int old_value;
    if (ARRAYLIST_SET(int_list, list, 1, 25, &old_value) == SUCCESS_int_list) {
        printf("Updated index 1: %d -> 25\n", old_value);
    }
    
    // Final state
    printf("Final list: ");
    for (size_t i = 0; i < ARRAYLIST_LEN(int_list, list); i++) {
        if (ARRAYLIST_GET(int_list, list, i, &value) == SUCCESS_int_list) {
            printf("%d ", value);
        }
    }
    printf("\n");
    
    // Clean up
    ARRAYLIST_DESTROY(int_list, list);
    
    return 0;
}
```

### Working with Different Types

```c
// Generate ArrayList for strings
GENERATE_ARRAYLIST(string_list, char*)

ArrayList_string_list *strings = ARRAYLIST_CREATE(string_list);
ARRAYLIST_ADD_LAST(string_list, strings, "Hello");
ARRAYLIST_ADD_LAST(string_list, strings, "World");

// Generate ArrayList for custom structs
typedef struct {
    int x, y;
} Point;

GENERATE_ARRAYLIST(point_list, Point)

ArrayList_point_list *points = ARRAYLIST_CREATE(point_list);
Point p = {10, 20};
ARRAYLIST_ADD_LAST(point_list, points, p);
```
