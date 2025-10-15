# ArrayList Macro API Documentation

This document describes the **user-facing API macros** for the generic `ArrayList` implementation.
You only need to use these macros. All others are internal and generated automatically by `GENERATE_ARRAYLIST(name, type)`.

## Setup

Before using any macro, generate a typed ArrayList for your data type:

```c
GENERATE_ARRAYLIST(Int, int)
```

This creates:

- `ArrayList_Int` struct
- Enum `ArrayListError_Int`
- All the necessary functions behind the user-facing macros.

Then you can use the API like so:

```c
ArrayList_Int *list = ARRAYLIST_CREATE(Int);
ARRAYLIST_ADD_LAST(Int, list, 42);
ARRAYLIST_DESTROY(Int, list);
```

## `ARRAYLIST_CREATE(name)`

**Signature**

```c
ArrayList_<name> *ARRAYLIST_CREATE(name);
```

**Description**

Creates a new `ArrayList_<name>` with an initial capacity of 10 elements.

**Example**

```c
ArrayList_Int *list = ARRAYLIST_CREATE(Int);
```

## `ARRAYLIST_DESTROY(name, arraylist)`

**Signature**

```c
void ARRAYLIST_DESTROY(name, ArrayList_<name> *arraylist);
```

**Description**

Frees the list's internal storage and the list itself.

**Example**

```c
ARRAYLIST_DESTROY(Int, list);
```

## `ARRAYLIST_COUNT(name, arraylist)`

**Signature**

```c
size_t ARRAYLIST_COUNT(name, ArrayList_<name> *arraylist);
```

**Description**

Returns the number of elements currently in the list.

**Example**

```c
size_t count = ARRAYLIST_COUNT(Int, list);
printf("Count = %zu\n", count);
```

## `ARRAYLIST_IS_EMPTY(name, arraylist)`

**Signature**

```c
bool ARRAYLIST_IS_EMPTY(name, ArrayList_<name> *arraylist);
```

**Description**

Checks whether the list is empty.

**Example**

```c
if (ARRAYLIST_IS_EMPTY(Int, list)) {
    printf("List is empty!\n");
}
```

## `ARRAYLIST_GET(name, arraylist, index, out)`

**Signature**

```c
ArrayListError_<name> ARRAYLIST_GET(name, ArrayList_<name> *arraylist, size_t index, type *out);
```

**Description**

Retrieves the element at a given index.
Returns an error code if the list is empty or the index is invalid.

**Example**

```c
int value;
if (ARRAYLIST_GET(Int, list, 2, &value) == SUCCESS_Int) {
    printf("Value at index 2: %d\n", value);
}
```

## `ARRAYLIST_GET_FIRST(name, arraylist, out)`

**Description**

Gets the first element of the list.

**Example**

```c
int first;
ARRAYLIST_GET_FIRST(Int, list, &first);
```

## `ARRAYLIST_GET_LAST(name, arraylist, out)`

**Description**

Gets the last element of the list.

**Example**

```c
int last;
ARRAYLIST_GET_LAST(Int, list, &last);
```

## `ARRAYLIST_SET(name, arraylist, index, new_element, out)`

**Description**

Replaces the element at `index` with `new_element`.
If `out` is non-NULL, stores the old value there.

**Example**

```c
int old;
ARRAYLIST_SET(Int, list, 1, 999, &old);
```

## `ARRAYLIST_ADD(name, arraylist, index, element)`

**Description**

Inserts an element at a specific index, shifting subsequent elements.

**Example**

```c
ARRAYLIST_ADD(Int, list, 2, 42);
```

## `ARRAYLIST_ADD_FIRST(name, arraylist, element)`

**Description**

Prepends an element to the beginning of the list.

**Example**

```c
ARRAYLIST_ADD_FIRST(Int, list, 10);
```

## `ARRAYLIST_ADD_LAST(name, arraylist, element)`

**Description**

Appends an element to the end of the list.

**Example**

```c
ARRAYLIST_ADD_LAST(Int, list, 20);
```

## `ARRAYLIST_REMOVE(name, arraylist, index, out)`

**Description**

Removes the element at a specific index.
If `out` is non-NULL, stores the removed element there.

**Example**

```c
int removed;
ARRAYLIST_REMOVE(Int, list, 1, &removed);
```

## `ARRAYLIST_REMOVE_FIRST(name, arraylist, out)`

**Description**

Removes the first element of the list.

**Example**

```c
int first;
ARRAYLIST_REMOVE_FIRST(Int, list, &first);
```

## `ARRAYLIST_REMOVE_LAST(name, arraylist, out)`

**Description**

Removes the last element of the list.

**Example**

```c
int last;
ARRAYLIST_REMOVE_LAST(Int, list, &last);
```

## Full Example

```c
#include "arraylist.h"
#include <stdio.h>

GENERATE_ARRAYLIST(Int, int)

int main(void) {
    ArrayList_Int *list = ARRAYLIST_CREATE(Int);

    ARRAYLIST_ADD_LAST(Int, list, 10);
    ARRAYLIST_ADD_LAST(Int, list, 20);
    ARRAYLIST_ADD_LAST(Int, list, 30);

    int value;
    ARRAYLIST_GET(Int, list, 1, &value);
    printf("Index 1 = %d\n", value);

    ARRAYLIST_SET(Int, list, 1, 99, NULL);

    ARRAYLIST_REMOVE_FIRST(Int, list, NULL);
    printf("Count after removal: %zu\n", ARRAYLIST_COUNT(Int, list));

    ARRAYLIST_DESTROY(Int, list);
    return 0;
}
```
