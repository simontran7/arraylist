# arraylist

C type-safe generic ArrayList

## Usage

`main.c`

```c
#include <stdio.h>
#include "arraylist.h" // Assuming it's in the same directory as `main.c`

// Generate an ArrayList of type `int`
GENERATE_ARRAYLIST(Int, int)

int main(void) {
    // Create list
    ArrayList_Int *list = ARRAYLIST_CREATE(Int);
    if (list == NULL) {
        fprintf(stderr, "Failed to create ArrayList\n");
        return 1;
    }

    // Add elements
    for (int i = 10; i < 20; i++) {
        if (ARRAYLIST_ADD_LAST(Int, list, i) != SUCCESS_Int) {
            fprintf(stderr, "Failed to add element %d\n", i);
        }
    }
    printf("After adding 10 elements:\n");
    for (size_t i = 0; i < ARRAYLIST_COUNT(Int, list); i++) {
        int val;
        ARRAYLIST_GET(Int, list, i, &val);
        printf("[%zu] = %d\n", i, val);
    }

    // Insert in the middle of the list
    ARRAYLIST_ADD(Int, list, 2, 99);
    printf("\nAfter inserting 99 at index 2:\n");
    for (size_t i = 0; i < ARRAYLIST_COUNT(Int, list); i++) {
        int val;
        ARRAYLIST_GET(Int, list, i, &val);
        printf("[%zu] = %d\n", i, val);
    }

    // Modify element
    int old;
    ARRAYLIST_SET(Int, list, 3, 1234, &old);
    printf("\nSet index 3 to 1234 (old value was %d):\n", old);
    for (size_t i = 0; i < ARRAYLIST_COUNT(Int, list); i++) {
        int val;
        ARRAYLIST_GET(Int, list, i, &val);
        printf("[%zu] = %d\n", i, val);
    }

    // Remove element
    int removed;
    ARRAYLIST_REMOVE(Int, list, 1, &removed);
    printf("\nRemoved index 1 (value %d):\n", removed);
    for (size_t i = 0; i < ARRAYLIST_COUNT(Int, list); i++) {
        int val;
        ARRAYLIST_GET(Int, list, i, &val);
        printf("[%zu] = %d\n", i, val);
    }

    // Destroy list
    ARRAYLIST_DESTROY(Int, list);

    return EXIT_SUCCESS;
}
```

`STDOUT`

```
After adding 10 elements:
[0] = 10
[1] = 11
[2] = 12
[3] = 13
[4] = 14
[5] = 15
[6] = 16
[7] = 17
[8] = 18
[9] = 19

After inserting 99 at index 2:
[0] = 10
[1] = 11
[2] = 99
[3] = 12
[4] = 13
[5] = 14
[6] = 15
[7] = 16
[8] = 17
[9] = 18
[10] = 19

Set index 3 to 1234 (old value was 12):
[0] = 10
[1] = 11
[2] = 99
[3] = 1234
[4] = 13
[5] = 14
[6] = 15
[7] = 16
[8] = 17
[9] = 18
[10] = 19

Removed index 1 (value 11):
[0] = 10
[1] = 99
[2] = 1234
[3] = 13
[4] = 14
[5] = 15
[6] = 16
[7] = 17
[8] = 18
[9] = 19
```

