#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/*
 * Initial capacity allocated when creating a new ArrayList.
 */
#define INITIAL_CAP 10

/*
 * Factor by which the array capacity grows when the underlying `data` pointer is full.
 */
#define GROWTH_FACTOR 1.5

/*
 * Generate `struct ArrayList_<name>`.
 */
#define GENERATE_ARRAYLIST_STRUCT(name, type) \
    typedef struct ArrayList_##name           \
    {                                         \
        size_t count;                         \
        size_t cap;                           \
        type *data;                           \
    } ArrayList_##name;

/*
 * Generates `enum ArrayListError_<name>`.
 */
#define GENERATE_ARRAYLIST_ERROR_ENUM(name) \
    typedef enum ArrayListError_##name{     \
        SUCCESS_##name,                     \
        EMPTY_ARRAYLIST_ERROR_##name,       \
        INDEX_OUT_OF_BOUNDS_ERROR_##name,   \
        MEMORY_ERROR_##name,                \
    } ArrayListError_##name;

/*
 * Generates `ArrayList_<name> *arraylist_create_<name>()`.
 */
#define GENERATE_ARRAYLIST_CREATE(name, type)                           \
    ArrayList_##name *arraylist_create_##name()                         \
    {                                                                   \
        ArrayList_##name *arraylist = malloc(sizeof(ArrayList_##name)); \
        if (!arraylist)                                                 \
            return NULL;                                                \
        arraylist->data = malloc(sizeof(type) * INITIAL_CAP);           \
        if (!arraylist->data)                                           \
        {                                                               \
            free(arraylist);                                            \
            return NULL;                                                \
        }                                                               \
        arraylist->count = 0;                                           \
        arraylist->cap = INITIAL_CAP;                                   \
        return arraylist;                                               \
    }

/*
 * Generates `void arraylist_destroy_<name>(ArrayList_<name> *arraylist)`.
 */
#define GENERATE_ARRAYLIST_DESTROY(name)                       \
    void arraylist_destroy_##name(ArrayList_##name *arraylist) \
    {                                                          \
        free(arraylist->data);                                 \
        free(arraylist);                                       \
    }

/*
 * Generates `size_t arraylist_count_<name>(ArrayList_<name> *arraylist)`.
 */
#define GENERATE_ARRAYLIST_COUNT(name)                         \
    size_t arraylist_count_##name(ArrayList_##name *arraylist) \
    {                                                          \
        return arraylist->count;                               \
    }

/*
 * Generates `bool arraylist_is_empty_<name>(ArrayList_<name> *arraylist)`.
 */
#define GENERATE_ARRAYLIST_IS_EMPTY(name)                       \
    bool arraylist_is_empty_##name(ArrayList_##name *arraylist) \
    {                                                           \
        return arraylist->count == 0;                           \
    }

/*
 * Generates `ArrayListError_<name> arraylist_get_<name>(ArrayList_<name> *arraylist, size_t index, type *out)`.
 */
#define GENERATE_ARRAYLIST_GET(name, type)                    \
    ArrayListError_##name arraylist_get_##name(               \
        ArrayList_##name *arraylist, size_t index, type *out) \
    {                                                         \
        if (arraylist->count == 0)                            \
            return EMPTY_ARRAYLIST_ERROR_##name;              \
        if (index >= arraylist->count)                        \
            return INDEX_OUT_OF_BOUNDS_ERROR_##name;          \
        *out = arraylist->data[index];                        \
        return SUCCESS_##name;                                \
    }

/*
 * Generates `ArrayListError_<name> arraylist_set_<name>(ArrayList_<name> *arraylist, size_t index, type new_element, type *out)`.
 */
#define GENERATE_ARRAYLIST_SET(name, type)           \
    ArrayListError_##name arraylist_set_##name(      \
        ArrayList_##name *arraylist, size_t index,   \
        type new_element, type *out)                 \
    {                                                \
        if (arraylist->count == 0)                   \
            return EMPTY_ARRAYLIST_ERROR_##name;     \
        if (index >= arraylist->count)               \
            return INDEX_OUT_OF_BOUNDS_ERROR_##name; \
        *out = arraylist->data[index];               \
        arraylist->data[index] = new_element;        \
        return SUCCESS_##name;                       \
    }

/*
 * Generates `ArrayListError_<name> arraylist_get_first_<name>(ArrayList_<name> *arraylist, type *out)`.
 */
#define GENERATE_ARRAYLIST_GET_FIRST(name, type)      \
    ArrayListError_##name arraylist_get_first_##name( \
        ArrayList_##name *arraylist, type *out)       \
    {                                                 \
        if (arraylist->count == 0)                    \
            return EMPTY_ARRAYLIST_ERROR_##name;      \
        *out = arraylist->data[0];                    \
        return SUCCESS_##name;                        \
    }

/*
 * Generates `ArrayListError_<name> arraylist_get_last_<name>(ArrayList_<name> *arraylist, type *out)`.
 */
#define GENERATE_ARRAYLIST_GET_LAST(name, type)       \
    ArrayListError_##name arraylist_get_last_##name(  \
        ArrayList_##name *arraylist, type *out)       \
    {                                                 \
        if (arraylist->count == 0)                    \
            return EMPTY_ARRAYLIST_ERROR_##name;      \
        *out = arraylist->data[arraylist->count - 1]; \
        return SUCCESS_##name;                        \
    }

/*
 * Generates `ArrayListError_<name> arraylist_grow_<name>(ArrayList_<name> *arraylist, size_t new_cap)`.
 */
#define GENERATE_ARRAYLIST_GROW(name, type)                                 \
    ArrayListError_##name arraylist_grow_##name(                            \
        ArrayList_##name *arraylist, size_t new_cap)                        \
    {                                                                       \
        type *new_array = realloc(arraylist->data, sizeof(type) * new_cap); \
        if (!new_array)                                                     \
            return MEMORY_ERROR_##name;                                     \
        arraylist->data = new_array;                                        \
        arraylist->cap = new_cap;                                           \
        return SUCCESS_##name;                                              \
    }

/*
 * Generates `ArrayListError_<name> arraylist_add_<name>(ArrayList_<name> *arraylist, size_t index, type element)`.
 */
#define GENERATE_ARRAYLIST_ADD(name, type)                            \
    ArrayListError_##name arraylist_add_##name(                       \
        ArrayList_##name *arraylist, size_t index, type element)      \
    {                                                                 \
        if (index > arraylist->count)                                 \
            return INDEX_OUT_OF_BOUNDS_ERROR_##name;                  \
        if (arraylist->count == arraylist->cap)                       \
        {                                                             \
            ArrayListError_##name res = arraylist_grow_##name(        \
                arraylist, (size_t)(arraylist->cap * GROWTH_FACTOR)); \
            if (res != SUCCESS_##name)                                \
                return res;                                           \
        }                                                             \
        memmove(&arraylist->data[index + 1], &arraylist->data[index], \
                (arraylist->count - index) * sizeof(type));           \
        arraylist->data[index] = element;                             \
        arraylist->count += 1;                                        \
        return SUCCESS_##name;                                        \
    }

/*
 * Generates `ArrayListError_<name> arraylist_add_first_<name>(ArrayList_<name> *arraylist, type element)`.
 */
#define GENERATE_ARRAYLIST_ADD_FIRST(name, type)            \
    ArrayListError_##name arraylist_add_first_##name(       \
        ArrayList_##name *arraylist, type element)          \
    {                                                       \
        return arraylist_add_##name(arraylist, 0, element); \
    }

/*
 * Generates `ArrayListError_<name> arraylist_add_last_<name>(ArrayList_<name> *arraylist, type element)`.
 */
#define GENERATE_ARRAYLIST_ADD_LAST(name, type)                            \
    ArrayListError_##name arraylist_add_last_##name(                       \
        ArrayList_##name *arraylist, type element)                         \
    {                                                                      \
        return arraylist_add_##name(arraylist, arraylist->count, element); \
    }

/*
 * Generates `ArrayListError_<name> arraylist_remove_first_<name>(ArrayList_<name> *arraylist, type *out)`.
 */
#define GENERATE_ARRAYLIST_REMOVE_FIRST(name, type)        \
    ArrayListError_##name arraylist_remove_first_##name(   \
        ArrayList_##name *arraylist, type *out)            \
    {                                                      \
        return arraylist_remove_##name(arraylist, 0, out); \
    }

/*
 * Generates `ArrayListError_<name> arraylist_remove_last_<name>(ArrayList_<name> *arraylist, type *out)`.
 */
#define GENERATE_ARRAYLIST_REMOVE_LAST(name, type)                            \
    ArrayListError_##name arraylist_remove_last_##name(                       \
        ArrayList_##name *arraylist, type *out)                               \
    {                                                                         \
        return arraylist_remove_##name(arraylist, arraylist->count - 1, out); \
    }

/*
 * Generates `ArrayListError_<name> arraylist_remove_first_<name>(ArrayList_<name> *arraylist, type *out)`.
 */
#define GENERATE_ARRAYLIST_REMOVE(name, type)                         \
    ArrayListError_##name arraylist_remove_##name(                    \
        ArrayList_##name *arraylist, size_t index, type *out)         \
    {                                                                 \
        if (arraylist->count == 0)                                    \
            return EMPTY_ARRAYLIST_ERROR_##name;                      \
        if (index >= arraylist->count)                                \
            return INDEX_OUT_OF_BOUNDS_ERROR_##name;                  \
        *out = arraylist->data[index];                                \
        memmove(&arraylist->data[index], &arraylist->data[index + 1], \
                (arraylist->count - index - 1) * sizeof(type));       \
        arraylist->count -= 1;                                        \
        return SUCCESS_##name;                                        \
    }

/*
 * Description:
 * Generates the full implementation of an ArrayList for a given type, including all functions and structures.
 *
 * Macro Parameters:
 * - `name`: An identifier used to distinguish different ArrayList types.
 * - `type`: The type of the ArrayList's elements.
 */
#define GENERATE_ARRAYLIST(name, type)          \
    GENERATE_ARRAYLIST_STRUCT(name, type)       \
    GENERATE_ARRAYLIST_ERROR_ENUM(name)         \
    GENERATE_ARRAYLIST_CREATE(name, type)       \
    GENERATE_ARRAYLIST_DESTROY(name)            \
    GENERATE_ARRAYLIST_COUNT(name)              \
    GENERATE_ARRAYLIST_IS_EMPTY(name)           \
    GENERATE_ARRAYLIST_GET(name, type)          \
    GENERATE_ARRAYLIST_GET_FIRST(name, type)    \
    GENERATE_ARRAYLIST_GET_LAST(name, type)     \
    GENERATE_ARRAYLIST_SET(name, type)          \
    GENERATE_ARRAYLIST_GROW(name, type)         \
    GENERATE_ARRAYLIST_ADD(name, type)          \
    GENERATE_ARRAYLIST_ADD_FIRST(name, type)    \
    GENERATE_ARRAYLIST_ADD_LAST(name, type)     \
    GENERATE_ARRAYLIST_REMOVE(name, type)       \
    GENERATE_ARRAYLIST_REMOVE_FIRST(name, type) \
    GENERATE_ARRAYLIST_REMOVE_LAST(name, type)

/*
 * Description:
 * Creates and returns a pointer to a new, heap-allocated instance of `ArrayList`.
 * The caller is responsible for freeing the memory using `ARRAYLIST_DESTROY`.
 *
 * Parameters:
 * - `name`: The identifier used to specialize the generated ArrayList type.
 *
 * Returns:
 * - A pointer to a new ArrayList instance, or NULL if allocation fails.
 */
#define ARRAYLIST_CREATE(name) arraylist_create_##name()

/*
 * Description:
 * Frees all memory associated with the ArrayList, including its internal data
 * array and the struct itself. After calling this function, the arraylist pointer becomes invalid.
 *
 * Parameters:
 * - `name`: The identifier used to specialize the generated ArrayList type.
 * - `arraylist`: Pointer to the arraylist instance to destroy.
 *
 * Returns:
 * - Nothing.
 */
#define ARRAYLIST_DESTROY(name, arraylist) arraylist_destroy_##name(arraylist)

/*
 * Description:
 * Returns the number of elements currently stored in the ArrayList.
 *
 * Parameters:
 * - `name`: The identifier used to specialize the generated ArrayList type.
 * - `arraylist`: Pointer to the arraylist instance.
 *
 * Returns:
 * - Number of elements as a `size_t`.
 */
#define ARRAYLIST_COUNT(name, arraylist) arraylist_count_##name(arraylist)

/*
 * Description:
 * Checks if the ArrayList is empty.
 *
 * Parameters:
 * - `name`: The identifier used to specialize the generated ArrayList type.
 * - `arraylist`: Pointer to the arraylist instance.
 *
 * Returns:
 * - `true` if the arraylist is empty.
 * - `false` otherwise.
 */
#define ARRAYLIST_IS_EMPTY(name, arraylist) arraylist_is_empty_##name(arraylist)

/*
 * Description:
 * Retrieves the element at the specified index in the arraylist.
 *
 * Parameters:
 * - `name`: The identifier used to specialize the generated ArrayList type.
 * - `arraylist`: Pointer to the arraylist instance.
 * - `index`: Zero-based index of the element to retrieve.
 * - `out`: Pointer where the retrieved element will be stored.
 *
 * Returns:
 * - `SUCCESS`
 * - `EMPTY_ARRAYLIST_ERROR`
 * - `INDEX_OUT_OF_BOUNDS_ERROR`
 */
#define ARRAYLIST_GET(name, arraylist, index, out) arraylist_get_##name(arraylist, index, out)

/*
 * Description:
 * Retrieves the first element in the ArrayList.
 *
 * Parameters:
 * - `name`: The identifier used to specialize the generated ArrayList type.
 * - `arraylist`: Pointer to the arraylist instance.
 * - `out`: Pointer where the retrieved element will be stored.
 *
 * Returns:
 * - `SUCCESS`
 * - `EMPTY_ARRAYLIST_ERROR`
 */
#define ARRAYLIST_GET_FIRST(name, arraylist, out) arraylist_get_first_##name(arraylist, out)

/*
 * Description:
 * Retrieves the last element in the ArrayList.
 *
 * Parameters:
 * - `name`: The identifier used to specialize the generated ArrayList type.
 * - `arraylist`: Pointer to the arraylist instance.
 * - `out`: Pointer where the retrieved element will be stored.
 *
 * Returns:
 * - `SUCCESS`
 * - `EMPTY_ARRAYLIST_ERROR`
 */
#define ARRAYLIST_GET_LAST(name, arraylist, out) arraylist_get_last_##name(arraylist, out)

/*
 * Description:
 * Updates the element at the specified index with a new value and returns
 * the previous value through the output parameter.
 *
 * Parameters:
 * - `name`: The identifier used to specialize the generated ArrayList type.
 * - `arraylist`: Pointer to the arraylist instance.
 * - `index`: Index of the element to update.
 * - `new_element`: New value to assign at the specified index.
 * - `out`: Pointer where the old value will be stored.
 *
 * Returns:
 * - `SUCCESS`
 * - `EMPTY_ARRAYLIST_ERROR`
 * - `INDEX_OUT_OF_BOUNDS_ERROR`
 */
#define ARRAYLIST_SET(name, arraylist, index, new_element, out) arraylist_set_##name(arraylist, index, new_element, out)

/*
 * Description:
 * Inserts a new element at the specified index, shifting subsequent elements to the right.
 * The arraylist automatically grows if at capacity.
 *
 * Parameters:
 * - `name`: The identifier used to specialize the generated ArrayList type.
 * - `arraylist`: Pointer to the arraylist instance.
 * - `index`: Index at which to insert the new element (0 <= index <= length).
 * - `element`: Element to insert.
 *
 * Returns:
 * - `SUCCESS`
 * - `INDEX_OUT_OF_BOUNDS_ERROR`
 * - `MEMORY_ERROR`
 */
#define ARRAYLIST_ADD(name, arraylist, index, element) arraylist_add_##name(arraylist, index, element)

/*
 * Description:
 * Inserts a new element at the beginning of the arraylist, shifting all existing elements to the right.
 *
 * Parameters:
 * - `name`: The identifier used to specialize the generated ArrayList type.
 * - `arraylist`: Pointer to the arraylist instance.
 * - `element`: Element to insert at the beginning.
 *
 * Returns:
 * - `SUCCESS`
 * - `MEMORY_ERROR`
 */
#define ARRAYLIST_ADD_FIRST(name, arraylist, element) arraylist_add_first_##name(arraylist, element)

/*
 * Description:
 * Appends a new element to the end of the arraylist. This is the most efficient insert operation.
 *
 * Parameters:
 * - `name`: The identifier used to specialize the generated ArrayList type.
 * - `arraylist`: Pointer to the arraylist instance.
 * - `element`: Element to append to the end.
 *
 * Returns:
 * - `SUCCESS`
 * - `MEMORY_ERROR`
 */
#define ARRAYLIST_ADD_LAST(name, arraylist, element) arraylist_add_last_##name(arraylist, element)

/*
 * Description:
 * Removes the element at the specified index and returns it through the
 * output parameter. Elements after the removed index are shifted left
 * to fill the gap. The array capacity is not reduced.
 *
 * Parameters:
 * - `arraylist`: Pointer to an instance of `ArrayList`.
 * - `index`: Zero-based index of the element to remove.
 * - `out`: Pointer where the removed element will be stored.
 *
 * Returns:
 * - `SUCCESS`
 * - `EMPTY_ARRAYLIST_ERROR`
 * - `INDEX_OUT_OF_BOUNDS_ERROR`
 * */
#define ARRAYLIST_REMOVE(name, arraylist, index, out) arraylist_remove_##name(arraylist, index, out)

/*
 * Description:
 * Removes the first element of the arraylist and returns it through the output parameter.
 * Equivalent to calling `ARRAYLIST_REMOVE(..., 0, ...)`.
 *
 * Parameters:
 * - `name`: The identifier used to specialize the generated ArrayList type.
 * - `arraylist`: Pointer to the arraylist instance.
 * - `out`: Pointer where the removed element will be stored.
 *
 * Returns:
 * - `SUCCESS`
 * - `EMPTY_ARRAYLIST_ERROR`
 */
#define ARRAYLIST_REMOVE_FIRST(name, arraylist, out) arraylist_remove_first_##name(arraylist, out)

/*
 * Description:
 * Removes the last element of the arraylist and returns it through the output parameter.
 * Equivalent to calling `ARRAYLIST_REMOVE(..., count - 1, ...)`.
 *
 * Parameters:
 * - `name`: The identifier used to specialize the generated ArrayList type.
 * - `arraylist`: Pointer to the arraylist instance.
 * - `out`: Pointer where the removed element will be stored.
 *
 * Returns:
 * - `SUCCESS`
 * - `EMPTY_ARRAYLIST_ERROR`
 */
#define ARRAYLIST_REMOVE_LAST(name, arraylist, out) arraylist_remove_last_##name(arraylist, out)

#endif // ARRAYLIST_H
