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
 * Factor by which the array capacity grows when resizing is needed.
 */
#define GROWTH_FACTOR 1.5

/*
 * Generate `struct ArrayList_<name>`.
 */
#define GENERATE_ARRAYLIST_STRUCT(name, type) \
    typedef struct ArrayList_##name           \
    {                                         \
        size_t len;                           \
        size_t cap;                           \
        type *data;                           \
    } ArrayList_##name;

/*
 * Generates `enum ArrayListError_<name>`.
 */
#define GENERATE_ARRAYLIST_ERROR_ENUM(name) \
    typedef enum ArrayListError_##name{     \
        SUCCESS_##name,                     \
        EMPTY_LIST_ERROR_##name,            \
        INDEX_OUT_OF_BOUNDS_ERROR_##name,   \
        MEMORY_ERROR_##name,                \
    } ArrayListError_##name;

/*
 * Generates `ArrayList_<name> *arraylist_create_<name>()`.
 */
#define GENERATE_ARRAYLIST_CREATE(name, type)                      \
    ArrayList_##name *arraylist_create_##name()                    \
    {                                                              \
        ArrayList_##name *list = malloc(sizeof(ArrayList_##name)); \
        if (!list)                                                 \
            return NULL;                                           \
        list->data = malloc(sizeof(type) * INITIAL_CAP);           \
        if (!list->data)                                           \
        {                                                          \
            free(list);                                            \
            return NULL;                                           \
        }                                                          \
        list->len = 0;                                             \
        list->cap = INITIAL_CAP;                                   \
        return list;                                               \
    }

/*
 * Generates `void arraylist_destroy_<name>(ArrayList_<name> *list)`.
 */
#define GENERATE_ARRAYLIST_DESTROY(name)                  \
    void arraylist_destroy_##name(ArrayList_##name *list) \
    {                                                     \
        free(list->data);                                 \
        free(list);                                       \
    }

/*
 * Generates `size_t arraylist_len_<name>(ArrayList_<name> *list)`.
 */
#define GENERATE_ARRAYLIST_LEN(name)                    \
    size_t arraylist_len_##name(ArrayList_##name *list) \
    {                                                   \
        return list->len;                               \
    }

/*
 * Generates `bool arraylist_is_empty_<name>(ArrayList_<name> *list)`.
 */
#define GENERATE_ARRAYLIST_IS_EMPTY(name)                  \
    bool arraylist_is_empty_##name(ArrayList_##name *list) \
    {                                                      \
        return list->len == 0;                             \
    }

/*
 * Generates `ArrayListError_<name> arraylist_get_<name>(ArrayList_<name> *list, size_t index, type *out)`.
 */
#define GENERATE_ARRAYLIST_GET(name, type)               \
    ArrayListError_##name arraylist_get_##name(          \
        ArrayList_##name *list, size_t index, type *out) \
    {                                                    \
        if (list->len == 0)                              \
            return EMPTY_LIST_ERROR_##name;              \
        if (index >= list->len)                          \
            return INDEX_OUT_OF_BOUNDS_ERROR_##name;     \
        *out = list->data[index];                        \
        return SUCCESS_##name;                           \
    }

/*
 * Generates `ArrayListError_<name> arraylist_set_<name>(ArrayList_<name> *list, size_t index, type new_element, type *out)`.
 */
#define GENERATE_ARRAYLIST_SET(name, type)           \
    ArrayListError_##name arraylist_set_##name(      \
        ArrayList_##name *list, size_t index,        \
        type new_element, type *out)                 \
    {                                                \
        if (list->len == 0)                          \
            return EMPTY_LIST_ERROR_##name;          \
        if (index >= list->len)                      \
            return INDEX_OUT_OF_BOUNDS_ERROR_##name; \
        *out = list->data[index];                    \
        list->data[index] = new_element;             \
        return SUCCESS_##name;                       \
    }

/*
 * Generates `ArrayListError_<name> arraylist_get_first_<name>(ArrayList_<name> *list, type *out)`.
 */
#define GENERATE_ARRAYLIST_GET_FIRST(name, type)      \
    ArrayListError_##name arraylist_get_first_##name( \
        ArrayList_##name *list, type *out)            \
    {                                                 \
        if (list->len == 0)                           \
            return EMPTY_LIST_ERROR_##name;           \
        *out = list->data[0];                         \
        return SUCCESS_##name;                        \
    }

/*
 * Generates `ArrayListError_<name> arraylist_get_last_<name>(ArrayList_<name> *list, type *out)`.
 */
#define GENERATE_ARRAYLIST_GET_LAST(name, type)      \
    ArrayListError_##name arraylist_get_last_##name( \
        ArrayList_##name *list, type *out)           \
    {                                                \
        if (list->len == 0)                          \
            return EMPTY_LIST_ERROR_##name;          \
        *out = list->data[list->len - 1];            \
        return SUCCESS_##name;                       \
    }

/*
 * Generates `ArrayListError_<name> arraylist_resize_<name>(ArrayList_<name> *list, size_t new_cap)`.
 */
#define GENERATE_ARRAYLIST_RESIZE(name, type)                          \
    ArrayListError_##name arraylist_resize_##name(                     \
        ArrayList_##name *list, size_t new_cap)                        \
    {                                                                  \
        type *new_array = realloc(list->data, sizeof(type) * new_cap); \
        if (!new_array)                                                \
            return MEMORY_ERROR_##name;                                \
        list->data = new_array;                                        \
        list->cap = new_cap;                                           \
        return SUCCESS_##name;                                         \
    }

/*
 * Generates `ArrayListError_<name> arraylist_add_<name>(ArrayList_<name> *list, size_t index, type element)`.
 */
#define GENERATE_ARRAYLIST_ADD(name, type)                       \
    ArrayListError_##name arraylist_add_##name(                  \
        ArrayList_##name *list, size_t index, type element)      \
    {                                                            \
        if (index > list->len)                                   \
            return INDEX_OUT_OF_BOUNDS_ERROR_##name;             \
        if (list->len == list->cap)                              \
        {                                                        \
            ArrayListError_##name res = arraylist_resize_##name( \
                list, (size_t)(list->cap * GROWTH_FACTOR));      \
            if (res != SUCCESS_##name)                           \
                return res;                                      \
        }                                                        \
        memmove(&list->data[index + 1], &list->data[index],      \
                (list->len - index) * sizeof(type));             \
        list->data[index] = element;                             \
        list->len += 1;                                          \
        return SUCCESS_##name;                                   \
    }

/*
 * Generates `ArrayListError_<name> arraylist_add_first_<name>(ArrayList_<name> *list, type element)`.
 */
#define GENERATE_ARRAYLIST_ADD_FIRST(name, type)       \
    ArrayListError_##name arraylist_add_first_##name(  \
        ArrayList_##name *list, type element)          \
    {                                                  \
        return arraylist_add_##name(list, 0, element); \
    }

/*
 * Generates `ArrayListError_<name> arraylist_add_last_<name>(ArrayList_<name> *list, type element)`.
 */
#define GENERATE_ARRAYLIST_ADD_LAST(name, type)                \
    ArrayListError_##name arraylist_add_last_##name(           \
        ArrayList_##name *list, type element)                  \
    {                                                          \
        return arraylist_add_##name(list, list->len, element); \
    }

/*
 * Generates `ArrayListError_<name> arraylist_remove_first_<name>(ArrayList_<name> *list, type *out)`.
 */
#define GENERATE_ARRAYLIST_REMOVE_FIRST(name, type)      \
    ArrayListError_##name arraylist_remove_first_##name( \
        ArrayList_##name *list, type *out)               \
    {                                                    \
        return arraylist_remove_##name(list, 0, out);    \
    }

/*
 * Generates `ArrayListError_<name> arraylist_remove_last_<name>(ArrayList_<name> *list, type *out)`.
 */
#define GENERATE_ARRAYLIST_REMOVE_LAST(name, type)                \
    ArrayListError_##name arraylist_remove_last_##name(           \
        ArrayList_##name *list, type *out)                        \
    {                                                             \
        return arraylist_remove_##name(list, list->len - 1, out); \
    }

/*
 * Generates `ArrayListError_<name> arraylist_remove_first_<name>(ArrayList_<name> *list, type *out)`.
 */
#define GENERATE_ARRAYLIST_REMOVE(name, type)               \
    ArrayListError_##name arraylist_remove_##name(          \
        ArrayList_##name *list, size_t index, type *out)    \
    {                                                       \
        if (list->len == 0)                                 \
            return EMPTY_LIST_ERROR_##name;                 \
        if (index >= list->len)                             \
            return INDEX_OUT_OF_BOUNDS_ERROR_##name;        \
        *out = list->data[index];                           \
        memmove(&list->data[index], &list->data[index + 1], \
                (list->len - index - 1) * sizeof(type));    \
        list->len -= 1;                                     \
        return SUCCESS_##name;                              \
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
    GENERATE_ARRAYLIST_LEN(name)                \
    GENERATE_ARRAYLIST_IS_EMPTY(name)           \
    GENERATE_ARRAYLIST_GET(name, type)          \
    GENERATE_ARRAYLIST_GET_FIRST(name, type)    \
    GENERATE_ARRAYLIST_GET_LAST(name, type)     \
    GENERATE_ARRAYLIST_SET(name, type)          \
    GENERATE_ARRAYLIST_RESIZE(name, type)       \
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
 * array and the struct itself. After calling this function, the list pointer becomes invalid.
 *
 * Parameters:
 * - `name`: The identifier used to specialize the generated ArrayList type.
 * - `list`: Pointer to the list instance to destroy.
 *
 * Returns:
 * - Nothing.
 */
#define ARRAYLIST_DESTROY(name, list) arraylist_destroy_##name(list)

/*
 * Description:
 * Returns the number of elements currently stored in the ArrayList.
 *
 * Parameters:
 * - `name`: The identifier used to specialize the generated ArrayList type.
 * - `list`: Pointer to the list instance.
 *
 * Returns:
 * - Number of elements as a `size_t`.
 */
#define ARRAYLIST_LEN(name, list) arraylist_len_##name(list)

/*
 * Description:
 * Checks if the ArrayList is empty.
 *
 * Parameters:
 * - `name`: The identifier used to specialize the generated ArrayList type.
 * - `list`: Pointer to the list instance.
 *
 * Returns:
 * - `true` if the list is empty.
 * - `false` otherwise.
 */
#define ARRAYLIST_IS_EMPTY(name, list) arraylist_is_empty_##name(list)

/*
 * Description:
 * Retrieves the element at the specified index in the list.
 *
 * Parameters:
 * - `name`: The identifier used to specialize the generated ArrayList type.
 * - `list`: Pointer to the list instance.
 * - `index`: Zero-based index of the element to retrieve.
 * - `out`: Pointer where the retrieved element will be stored.
 *
 * Returns:
 * - `SUCCESS`
 * - `EMPTY_LIST_ERROR`
 * - `INDEX_OUT_OF_BOUNDS_ERROR`
 */
#define ARRAYLIST_GET(name, list, index, out) arraylist_get_##name(list, index, out)

/*
 * Description:
 * Retrieves the first element in the ArrayList.
 *
 * Parameters:
 * - `name`: The identifier used to specialize the generated ArrayList type.
 * - `list`: Pointer to the list instance.
 * - `out`: Pointer where the retrieved element will be stored.
 *
 * Returns:
 * - `SUCCESS`
 * - `EMPTY_LIST_ERROR`
 */
#define ARRAYLIST_GET_FIRST(name, list, out) arraylist_get_first_##name(list, out)

/*
 * Description:
 * Retrieves the last element in the ArrayList.
 *
 * Parameters:
 * - `name`: The identifier used to specialize the generated ArrayList type.
 * - `list`: Pointer to the list instance.
 * - `out`: Pointer where the retrieved element will be stored.
 *
 * Returns:
 * - `SUCCESS`
 * - `EMPTY_LIST_ERROR`
 */
#define ARRAYLIST_GET_LAST(name, list, out) arraylist_get_last_##name(list, out)

/*
 * Description:
 * Updates the element at the specified index with a new value and returns
 * the previous value through the output parameter.
 *
 * Parameters:
 * - `name`: The identifier used to specialize the generated ArrayList type.
 * - `list`: Pointer to the list instance.
 * - `index`: Index of the element to update.
 * - `new_element`: New value to assign at the specified index.
 * - `out`: Pointer where the old value will be stored.
 *
 * Returns:
 * - `SUCCESS`
 * - `EMPTY_LIST_ERROR`
 * - `INDEX_OUT_OF_BOUNDS_ERROR`
 */
#define ARRAYLIST_SET(name, list, index, new_element, out) arraylist_set_##name(list, index, new_element, out)

/*
 * Description:
 * Inserts a new element at the specified index, shifting subsequent elements to the right.
 * The list automatically resizes if at capacity.
 *
 * Parameters:
 * - `name`: The identifier used to specialize the generated ArrayList type.
 * - `list`: Pointer to the list instance.
 * - `index`: Index at which to insert the new element (0 ≤ index ≤ length).
 * - `element`: Element to insert.
 *
 * Returns:
 * - `SUCCESS`
 * - `INDEX_OUT_OF_BOUNDS_ERROR`
 * - `MEMORY_ERROR`
 */
#define ARRAYLIST_ADD(name, list, index, element) arraylist_add_##name(list, index, element)

/*
 * Description:
 * Inserts a new element at the beginning of the list, shifting all existing elements to the right.
 *
 * Parameters:
 * - `name`: The identifier used to specialize the generated ArrayList type.
 * - `list`: Pointer to the list instance.
 * - `element`: Element to insert at the beginning.
 *
 * Returns:
 * - `SUCCESS`
 * - `MEMORY_ERROR`
 */
#define ARRAYLIST_ADD_FIRST(name, list, element) arraylist_add_first_##name(list, element)

/*
 * Description:
 * Appends a new element to the end of the list. This is the most efficient insert operation.
 *
 * Parameters:
 * - `name`: The identifier used to specialize the generated ArrayList type.
 * - `list`: Pointer to the list instance.
 * - `element`: Element to append to the end.
 *
 * Returns:
 * - `SUCCESS`
 * - `MEMORY_ERROR`
 */
#define ARRAYLIST_ADD_LAST(name, list, element) arraylist_add_last_##name(list, element)

/*
 * Description:
 * Removes the element at the specified index and returns it through the
 * output parameter. Elements after the removed index are shifted left
 * to fill the gap. The array capacity is not reduced.
 *
 * Parameters:
 * - `list`: Pointer to an instance of `ArrayList`.
 * - `index`: Zero-based index of the element to remove.
 * - `out`: Pointer where the removed element will be stored.
 *
 * Returns:
 * - `SUCCESS`
 * - `EMPTY_LIST_ERROR`
 * - `INDEX_OUT_OF_BOUNDS_ERROR`
 * */
#define ARRAYLIST_REMOVE(name, list, index, out) arraylist_remove_##name(list, index, out)

/*
 * Description:
 * Removes the first element of the list and returns it through the output parameter.
 * Equivalent to calling `ARRAYLIST_REMOVE(..., 0, ...)`.
 *
 * Parameters:
 * - `name`: The identifier used to specialize the generated ArrayList type.
 * - `list`: Pointer to the list instance.
 * - `out`: Pointer where the removed element will be stored.
 *
 * Returns:
 * - `SUCCESS`
 * - `EMPTY_LIST_ERROR`
 */
#define ARRAYLIST_REMOVE_FIRST(name, list, out) arraylist_remove_first_##name(list, out)

/*
 * Description:
 * Removes the last element of the list and returns it through the output parameter.
 * Equivalent to calling `ARRAYLIST_REMOVE(..., len - 1, ...)`.
 *
 * Parameters:
 * - `name`: The identifier used to specialize the generated ArrayList type.
 * - `list`: Pointer to the list instance.
 * - `out`: Pointer where the removed element will be stored.
 *
 * Returns:
 * - `SUCCESS`
 * - `EMPTY_LIST_ERROR`
 */
#define ARRAYLIST_REMOVE_LAST(name, list, out) arraylist_remove_last_##name(list, out)

#endif // ARRAYLIST_H
