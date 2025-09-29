# arraylist

C type-safe generic ArrayList

> [!warning]
> Several safety bugs. Not fully usable (yet).

## Usage

The following is an example using the header file:

`main.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "arraylist.h" // Assuming it's in the same directory as `main.c`

// Generate a complete ArrayList implementation specialized for int,
// where the type will be `ArrayList_int`.
GENERATE_ARRAYLIST(int, int)

int main(void)
{
    // Create a new list of integers
    ArrayList_int *list = ARRAYLIST_CREATE(int);
    if (!list) {
        fprintf(stderr, "Failed to allocate ArrayList<int>\n");
        return EXIT_FAILURE;
    }

    // Append some numbers
    for (int i = 1; i <= 5; i++) {
        if (ARRAYLIST_ADD_LAST(int, list, i) != SUCCESS_int) {
            fprintf(stderr, "Failed to add %d\n", i);
            ARRAYLIST_DESTROY(int, list);
            return EXIT_FAILURE;
        }
    }

    // Read elements
    printf("Current elements (%zu): ", ARRAYLIST_COUNT(int, list));
    for (size_t i = 0; i < ARRAYLIST_COUNT(int, list); ++i) {
        int val;
        if (ARRAYLIST_GET(int, list, i, &val) == SUCCESS_int) {
            printf("%d ", val);
        }
    }
    printf("\n");

    // Update an element
    int old_val;
    if (ARRAYLIST_SET(int, list, 2, 42, &old_val) == SUCCESS_int) {
        printf("Replaced index 2 (old value %d) with 42\n", old_val);
    }

    // Remove the last element
    int removed;
    if (ARRAYLIST_REMOVE_LAST(int, list, &removed) == SUCCESS_int) {
        printf("Removed last element: %d\n", removed);
    }

    // Print final contents
    printf("Final list (%zu): ", ARRAYLIST_COUNT(int, list));
    for (size_t i = 0; i < ARRAYLIST_COUNT(int, list); ++i) {
        int val;
        ARRAYLIST_GET(int, list, i, &val);
        printf("%d ", val);
    }
    printf("\n");

    // Clean up
    ARRAYLIST_DESTROY(int, list);
    return EXIT_SUCCESS;
}
```

`STDOUT`

```c
Current elements (5): 1 2 3 4 5
Replaced index 2 (old value 3) with 42
Removed last element: 5
Final list (4): 1 2 42 4
```
