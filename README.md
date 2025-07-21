# arraylist

Type-safe generic ArrayList in C

## Example

`main.c`
```c
#include <stdio.h>
#include "arraylist.h"

// Generate ArrayList for type `int` with name `int`
GENERATE_ARRAYLIST(int, int)

int main() {
    // Create a new ArrayList of ints
    ArrayList_int *list = ARRAYLIST_CREATE(int);
    if (list == NULL) {
        fprintf(stderr, "Failed to allocate ArrayList\n");
        return 1;
    }

    // Add elements to the list
    for (int i = 1; i <= 5; i += 1) {
        if (ARRAYLIST_ADD_LAST(int, list, i) != SUCCESS_int) {
            fprintf(stderr, "Failed to add element %d\n", i);
            ARRAYLIST_DESTROY(int, list);
            return 1;
        }
    }

    // Get and print the first element
    int first;
    if (ARRAYLIST_GET_FIRST(int, list, &first) == SUCCESS_int) {
        printf("First element: %d\n", first);
    }

    // Get and print all elements
    printf("List contents: ");
    for (size_t i = 0; i < ARRAYLIST_LEN(int, list); i += 1) {
        int value;
        if (ARRAYLIST_GET(int, list, i, &value) == SUCCESS_int) {
            printf("%d ", value);
        }
    }
    printf("\n");

    // Remove the last element
    int removed;
    if (ARRAYLIST_REMOVE_LAST(int, list, &removed) == SUCCESS_int) {
        printf("Removed last element: %d\n", removed);
    }

    // Clean up
    ARRAYLIST_DESTROY(int, list);
    return 0;
}
```

`STDOUT`
```
First element: 1
List contents: 1 2 3 4 5 
Removed last element: 5
```
