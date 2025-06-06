/*
 * arraylist.h - Generic ArrayList implementation for C using macros
 * 
 * This header provides a type-safe, generic ArrayList implementation that can
 * work with any data type. It uses C preprocessor macros to generate the
 * necessary struct definitions and function implementations for each type.
 * 
 * Usage Example:
 *   // Generate ArrayList for integers
 *   GENERATE_ARRAYLIST(int_list, int)
 *   
 *   // Create and use the list
 *   ArrayList_int_list *list = ARRAYLIST_CREATE(int_list);
 *   ARRAYLIST_ADD_LAST(int_list, list, 42);
 */

#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/*
 * Initial capacity allocated when creating a new ArrayList
 * 
 * This determines how many elements can be stored before the first resize
 * operation is needed. Chosen to balance memory usage with performance.
 */
#define INITIAL_CAP 10

/*
 * Factor by which the array capacity grows when resizing is needed
 * 
 * When the array needs to grow, the new capacity = old_capacity * GROWTH_FACTOR.
 * A factor of 1.5 provides a good balance between memory usage and the frequency
 * of expensive resize operations. Values closer to 2.0 use more memory but
 * resize less frequently.
 */
#define GROWTH_FACTOR 1.5

/*
 * Generate the ArrayList struct definition for a specific type
 * name: Unique identifier for this ArrayList type (e.g., "int_list")
 * type: The data type to store (e.g., int, char*, struct MyStruct)
 * 
 * Creates a struct named ArrayList_<name> with:
 * - len: Current number of elements stored
 * - cap: Current capacity (allocated space)
 * - data: Pointer to the dynamically allocated array
 */
#define GENERATE_ARRAYLIST_STRUCT(name, type)                                  \
    typedef struct ArrayList_##name {                                          \
        size_t len;      // Current number of elements                         \
        size_t cap;      // Current allocated capacity                         \
        type *data;      // Pointer to the dynamic array                       \
    } ArrayList_##name;

/*
 * Generate error enum for ArrayList operations
 * name: Unique identifier matching the ArrayList type
 * 
 * Defines possible return values for ArrayList operations:
 * - SUCCESS: Operation completed successfully
 * - EMPTY_LIST_ERROR: Attempted operation on empty list (e.g., get/remove)
 * - INDEX_OUT_OF_BOUNDS_ERROR: Index parameter exceeds list bounds
 * - MEMORY_ERROR: Memory allocation/reallocation failed
 */
#define GENERATE_ARRAYLIST_ERROR_ENUM(name)                                    \
    typedef enum ArrayListError_##name {                                       \
        SUCCESS_##name,                    // Operation successful             \
        EMPTY_LIST_ERROR_##name,           // List is empty                    \
        INDEX_OUT_OF_BOUNDS_ERROR_##name,  // Invalid index                   \
        MEMORY_ERROR_##name,               // Memory allocation failed         \
    } ArrayListError_##name;

/*
 * Generate ArrayList constructor function
 * name: Unique identifier for this ArrayList type
 * type: The data type to store
 * 
 * Creates function: ArrayList_<name> *arraylist_create_<name>()
 * 
 * Allocates memory for both the ArrayList struct and its initial data array.
 * Returns NULL if memory allocation fails. The caller is responsible for
 * calling the corresponding destroy function to free memory.
 * 
 * Returns: Pointer to new ArrayList, or NULL on allocation failure
 */
#define GENERATE_ARRAYLIST_CREATE(name, type)                                  \
    ArrayList_##name *arraylist_create_##name() {                              \
        ArrayList_##name *list = malloc(sizeof(ArrayList_##name));             \
        if (!list) return NULL;                                                \
        list->data = malloc(sizeof(type) * INITIAL_CAP);                       \
        if (!list->data) {                                                     \
            free(list);                                                        \
            return NULL;                                                       \
        }                                                                      \
        list->len = 0;                                                         \
        list->cap = INITIAL_CAP;                                               \
        return list;                                                           \
    }

/*
 * Generate ArrayList destructor function
 * name: Unique identifier for this ArrayList type
 * 
 * Creates function: void arraylist_destroy_<name>(ArrayList_<name> *list)
 * 
 * Frees all memory associated with the ArrayList, including both the data
 * array and the ArrayList struct itself. After calling this function, the
 * list pointer becomes invalid and should not be used.
 * 
 * list: Pointer to ArrayList to destroy (must not be NULL)
 */
#define GENERATE_ARRAYLIST_DESTROY(name)                                       \
    void arraylist_destroy_##name(ArrayList_##name *list) {                    \
        free(list->data);                                                      \
        free(list);                                                            \
    }

/*
 * Generate function to get the number of elements in the ArrayList
 * name: Unique identifier for this ArrayList type
 * 
 * Creates function: size_t arraylist_len_<name>(ArrayList_<name> *list)
 * 
 * list: Pointer to ArrayList
 * Returns: Number of elements currently stored in the list
 */
#define GENERATE_ARRAYLIST_LEN(name)                                           \
    size_t arraylist_len_##name(ArrayList_##name *list) {                      \
        return list->len;                                                      \
    }

/*
 * Generate function to check if ArrayList is empty
 * name: Unique identifier for this ArrayList type
 * 
 * Creates function: bool arraylist_is_empty_<name>(ArrayList_<name> *list)
 * 
 * list: Pointer to ArrayList
 * Returns: true if the list contains no elements, false otherwise
 */
#define GENERATE_ARRAYLIST_IS_EMPTY(name)                                      \
    bool arraylist_is_empty_##name(ArrayList_##name *list) {                   \
        return list->len == 0;                                                 \
    }

/*
 * Generate function to retrieve an element at a specific index
 * name: Unique identifier for this ArrayList type
 * type: The data type stored in the ArrayList
 * 
 * Creates function: ArrayListError_<name> arraylist_get_<name>(
 *     ArrayList_<name> *list, size_t index, type *out)
 * 
 * Retrieves the element at the specified index and stores it in the output
 * parameter. This function performs bounds checking and handles empty lists.
 * 
 * list: Pointer to ArrayList
 * index: Zero-based index of element to retrieve
 * out: Pointer where the retrieved element will be stored
 * Returns: SUCCESS on success, EMPTY_LIST_ERROR or INDEX_OUT_OF_BOUNDS_ERROR on failure
 */
#define GENERATE_ARRAYLIST_GET(name, type)                                     \
    ArrayListError_##name arraylist_get_##name(                                \
        ArrayList_##name *list, size_t index, type *out) {                     \
        if (list->len == 0) return EMPTY_LIST_ERROR_##name;                    \
        if (index >= list->len) return INDEX_OUT_OF_BOUNDS_ERROR_##name;       \
        *out = list->data[index];                                              \
        return SUCCESS_##name;                                                 \
    }

/*
 * Generate function to update an element at a specific index
 * name: Unique identifier for this ArrayList type
 * type: The data type stored in the ArrayList
 * 
 * Creates function: ArrayListError_<name> arraylist_set_<name>(
 *     ArrayList_<name> *list, size_t index, type new_element, type *out)
 * 
 * Replaces the element at the specified index with a new value, and returns
 * the old value through the output parameter. Performs bounds checking.
 * 
 * list: Pointer to ArrayList
 * index: Zero-based index of element to update
 * new_element: New value to store at the index
 * out: Pointer where the old element value will be stored
 * Returns: SUCCESS on success, EMPTY_LIST_ERROR or INDEX_OUT_OF_BOUNDS_ERROR on failure
 */
#define GENERATE_ARRAYLIST_SET(name, type)                                     \
    ArrayListError_##name arraylist_set_##name(                                \
        ArrayList_##name *list, size_t index,                                  \
        type new_element, type *out) {                                         \
        if (list->len == 0) return EMPTY_LIST_ERROR_##name;                    \
        if (index >= list->len) return INDEX_OUT_OF_BOUNDS_ERROR_##name;       \
        *out = list->data[index];                                              \
        list->data[index] = new_element;                                       \
        return SUCCESS_##name;                                                 \
    }

/*
 * Generate function to retrieve the first element
 * name: Unique identifier for this ArrayList type
 * type: The data type stored in the ArrayList
 * 
 * Creates function: ArrayListError_<name> arraylist_get_first_<name>(
 *     ArrayList_<name> *list, type *out)
 * 
 * Convenience function equivalent to arraylist_get_<name>(list, 0, out).
 * 
 * list: Pointer to ArrayList
 * out: Pointer where the first element will be stored
 * Returns: SUCCESS on success, EMPTY_LIST_ERROR if list is empty
 */
#define GENERATE_ARRAYLIST_GET_FIRST(name, type)                               \
    ArrayListError_##name arraylist_get_first_##name(                          \
        ArrayList_##name *list, type *out) {                                   \
        if (list->len == 0) return EMPTY_LIST_ERROR_##name;                    \
        *out = list->data[0];                                                  \
        return SUCCESS_##name;                                                 \
    }

/*
 * Generate function to retrieve the last element
 * name: Unique identifier for this ArrayList type
 * type: The data type stored in the ArrayList
 * 
 * Creates function: ArrayListError_<name> arraylist_get_last_<name>(
 *     ArrayList_<name> *list, type *out)
 * 
 * Convenience function equivalent to arraylist_get_<name>(list, len-1, out).
 * 
 * list: Pointer to ArrayList
 * out: Pointer where the last element will be stored
 * Returns: SUCCESS on success, EMPTY_LIST_ERROR if list is empty
 */
#define GENERATE_ARRAYLIST_GET_LAST(name, type)                                \
    ArrayListError_##name arraylist_get_last_##name(                           \
        ArrayList_##name *list, type *out) {                                   \
        if (list->len == 0) return EMPTY_LIST_ERROR_##name;                    \
        *out = list->data[list->len - 1];                                      \
        return SUCCESS_##name;                                                 \
    }

/*
 * Generate function to resize the internal array
 * name: Unique identifier for this ArrayList type
 * type: The data type stored in the ArrayList
 * 
 * Creates function: ArrayListError_<name> arraylist_resize_<name>(
 *     ArrayList_<name> *list, size_t new_cap)
 * 
 * Internal function used to grow or shrink the capacity of the ArrayList.
 * Uses realloc() to change the size of the data array. If realloc fails,
 * the original array remains unchanged.
 * 
 * list: Pointer to ArrayList
 * new_cap: New capacity for the array
 * Returns: SUCCESS on success, MEMORY_ERROR if realloc fails
 */
#define GENERATE_ARRAYLIST_RESIZE(name, type)                                  \
    ArrayListError_##name arraylist_resize_##name(                             \
        ArrayList_##name *list, size_t new_cap) {                              \
        type *new_array = realloc(list->data, sizeof(type) * new_cap);         \
        if (!new_array) return MEMORY_ERROR_##name;                            \
        list->data = new_array;                                                \
        list->cap = new_cap;                                                   \
        return SUCCESS_##name;                                                 \
    }

/*
 * Generate function to insert an element at a specific index
 * name: Unique identifier for this ArrayList type
 * type: The data type stored in the ArrayList
 * 
 * Creates function: ArrayListError_<name> arraylist_add_<name>(
 *     ArrayList_<name> *list, size_t index, type element)
 * 
 * Inserts a new element at the specified index, shifting existing elements
 * to the right. If the array is at capacity, it will be automatically
 * resized. Index can be equal to the current length (append operation).
 * 
 * Time Complexity: O(n) due to element shifting
 * 
 * list: Pointer to ArrayList
 * index: Zero-based index where to insert (0 <= index <= len)
 * element: Element to insert
 * Returns: SUCCESS on success, INDEX_OUT_OF_BOUNDS_ERROR or MEMORY_ERROR on failure
 */
#define GENERATE_ARRAYLIST_ADD(name, type)                                     \
    ArrayListError_##name arraylist_add_##name(                                \
        ArrayList_##name *list, size_t index, type element) {                  \
        if (index > list->len) return INDEX_OUT_OF_BOUNDS_ERROR_##name;        \
        // Resize if at capacity                                               \
        if (list->len == list->cap) {                                          \
            ArrayListError_##name res = arraylist_resize_##name(               \
                list, (size_t) (list->cap * GROWTH_FACTOR));                   \
            if (res != SUCCESS_##name) return res;                             \
        }                                                                      \
        // Shift elements to the right to make space                          \
        memmove(&list->data[index + 1], &list->data[index],                    \
                (list->len - index) * sizeof(type));                           \
        list->data[index] = element;                                           \
        list->len += 1;                                                        \
        return SUCCESS_##name;                                                 \
    }

/*
 * Generate function to add element at the beginning
 * name: Unique identifier for this ArrayList type
 * type: The data type stored in the ArrayList
 * 
 * Creates function: ArrayListError_<name> arraylist_add_first_<name>(
 *     ArrayList_<name> *list, type element)
 * 
 * Convenience function equivalent to arraylist_add_<name>(list, 0, element).
 * Inserts element at index 0, shifting all existing elements to the right.
 * 
 * Time Complexity: O(n) due to element shifting
 * 
 * list: Pointer to ArrayList
 * element: Element to insert at the beginning
 * Returns: SUCCESS on success, MEMORY_ERROR if resize fails
 */
#define GENERATE_ARRAYLIST_ADD_FIRST(name, type)                               \
    ArrayListError_##name arraylist_add_first_##name(                          \
        ArrayList_##name *list, type element) {                                \
        return arraylist_add_##name(list, 0, element);                         \
    }

/*
 * Generate function to add element at the end
 * name: Unique identifier for this ArrayList type
 * type: The data type stored in the ArrayList
 * 
 * Creates function: ArrayListError_<name> arraylist_add_last_<name>(
 *     ArrayList_<name> *list, type element)
 * 
 * Convenience function equivalent to arraylist_add_<name>(list, len, element).
 * This is the most efficient insertion operation as no element shifting is needed.
 * 
 * Time Complexity: O(1) amortized (O(n) when resize is needed)
 * 
 * list: Pointer to ArrayList
 * element: Element to append to the end
 * Returns: SUCCESS on success, MEMORY_ERROR if resize fails
 */
#define GENERATE_ARRAYLIST_ADD_LAST(name, type)                                \
    ArrayListError_##name arraylist_add_last_##name(                           \
        ArrayList_##name *list, type element) {                                \
        return arraylist_add_##name(list, list->len, element);                 \
    }

/*
 * Generate function to remove the first element
 * name: Unique identifier for this ArrayList type
 * type: The data type stored in the ArrayList
 * 
 * Creates function: ArrayListError_<name> arraylist_remove_first_<name>(
 *     ArrayList_<name> *list, type *out)
 * 
 * Convenience function equivalent to arraylist_remove_<name>(list, 0, out).
 * Removes element at index 0, shifting all remaining elements to the left.
 * 
 * Time Complexity: O(n) due to element shifting
 * 
 * list: Pointer to ArrayList
 * out: Pointer where the removed element will be stored
 * Returns: SUCCESS on success, EMPTY_LIST_ERROR if list is empty
 */
#define GENERATE_ARRAYLIST_REMOVE_FIRST(name, type)                            \
    ArrayListError_##name arraylist_remove_first_##name(                       \
        ArrayList_##name *list, type *out) {                                   \
        return arraylist_remove_##name(list, 0, out);                          \
    }

/*
 * Generate function to remove the last element
 * name: Unique identifier for this ArrayList type
 * type: The data type stored in the ArrayList
 * 
 * Creates function: ArrayListError_<name> arraylist_remove_last_<name>(
 *     ArrayList_<name> *list, type *out)
 * 
 * Convenience function equivalent to arraylist_remove_<name>(list, len-1, out).
 * This is the most efficient removal operation as no element shifting is needed.
 * 
 * Time Complexity: O(1)
 * 
 * list: Pointer to ArrayList
 * out: Pointer where the removed element will be stored
 * Returns: SUCCESS on success, EMPTY_LIST_ERROR if list is empty
 */
#define GENERATE_ARRAYLIST_REMOVE_LAST(name, type)                             \
    ArrayListError_##name arraylist_remove_last_##name(                        \
        ArrayList_##name *list, type *out) {                                   \
        return arraylist_remove_##name(list, list->len - 1, out);              \
    }

/*
 * Generate function to remove element at a specific index
 * name: Unique identifier for this ArrayList type
 * type: The data type stored in the ArrayList
 * 
 * Creates function: ArrayListError_<name> arraylist_remove_<name>(
 *     ArrayList_<name> *list, size_t index, type *out)
 * 
 * Removes the element at the specified index and returns it through the
 * output parameter. Elements after the removed index are shifted left
 * to fill the gap. The array capacity is not reduced.
 * 
 * Time Complexity: O(n) due to element shifting (except for last element)
 * 
 * list: Pointer to ArrayList
 * index: Zero-based index of element to remove
 * out: Pointer where the removed element will be stored
 * Returns: SUCCESS on success, EMPTY_LIST_ERROR or INDEX_OUT_OF_BOUNDS_ERROR on failure
 */
#define GENERATE_ARRAYLIST_REMOVE(name, type)                                  \
    ArrayListError_##name arraylist_remove_##name(                             \
        ArrayList_##name *list, size_t index, type *out) {                     \
        if (list->len == 0) return EMPTY_LIST_ERROR_##name;                    \
        if (index >= list->len) return INDEX_OUT_OF_BOUNDS_ERROR_##name;       \
        *out = list->data[index];                                              \
        // Shift elements to the left to fill the gap                          \
        memmove(&list->data[index], &list->data[index + 1],                    \
                (list->len - index - 1) * sizeof(type));                       \
        list->len -= 1;                                                        \
        return SUCCESS_##name;                                                 \
    }

/*
 * Master macro to generate all ArrayList functions and types
 * name: Unique identifier for this ArrayList type
 * type: The data type to store
 * 
 * This macro generates all the necessary struct definitions, enums, and
 * function implementations for a complete ArrayList of the specified type.
 * 
 * Usage:
 *   GENERATE_ARRAYLIST(int_list, int)
 *   GENERATE_ARRAYLIST(string_list, char*)
 *   GENERATE_ARRAYLIST(point_list, struct Point)
 */
#define GENERATE_ARRAYLIST(name, type)                                         \
    GENERATE_ARRAYLIST_STRUCT(name, type)                                      \
    GENERATE_ARRAYLIST_ERROR_ENUM(name)                                        \
    GENERATE_ARRAYLIST_CREATE(name, type)                                      \
    GENERATE_ARRAYLIST_DESTROY(name)                                           \
    GENERATE_ARRAYLIST_LEN(name)                                               \
    GENERATE_ARRAYLIST_IS_EMPTY(name)                                          \
    GENERATE_ARRAYLIST_GET(name, type)                                         \
    GENERATE_ARRAYLIST_GET_FIRST(name, type)                                   \
    GENERATE_ARRAYLIST_GET_LAST(name, type)                                    \
    GENERATE_ARRAYLIST_SET(name, type)                                         \
    GENERATE_ARRAYLIST_RESIZE(name, type)                                      \
    GENERATE_ARRAYLIST_ADD(name, type)                                         \
    GENERATE_ARRAYLIST_ADD_FIRST(name, type)                                   \
    GENERATE_ARRAYLIST_ADD_LAST(name, type)                                    \
    GENERATE_ARRAYLIST_REMOVE(name, type)                                      \
    GENERATE_ARRAYLIST_REMOVE_FIRST(name, type)                                \
    GENERATE_ARRAYLIST_REMOVE_LAST(name, type)

/*
 * ============================================================================
 * CONVENIENCE MACROS FOR FUNCTION CALLS
 * ============================================================================
 * 
 * These macros provide a cleaner, more readable interface for calling the
 * generated functions. They hide the name mangling that occurs with the
 * macro-generated function names.
 */

// Create a new ArrayList instance
#define ARRAYLIST_CREATE(name) arraylist_create_##name()

// Destroy an ArrayList and free its memory
#define ARRAYLIST_DESTROY(name, list) arraylist_destroy_##name(list)

// Get the number of elements in the ArrayList
#define ARRAYLIST_LEN(name, list) arraylist_len_##name(list)

// Check if the ArrayList is empty
#define ARRAYLIST_IS_EMPTY(name, list) arraylist_is_empty_##name(list)

// Get element at specific index
#define ARRAYLIST_GET(name, list, index, out) arraylist_get_##name(list, index, out)

// Get the first element
#define ARRAYLIST_GET_FIRST(name, list, out) arraylist_get_first_##name(list, out)

// Get the last element
#define ARRAYLIST_GET_LAST(name, list, out) arraylist_get_last_##name(list, out)

// Set element at specific index
#define ARRAYLIST_SET(name, list, index, new_element, out) arraylist_set_##name(list, index, new_element, out)

// Add element at specific index
#define ARRAYLIST_ADD(name, list, index, element) arraylist_add_##name(list, index, element)

// Add element at the beginning
#define ARRAYLIST_ADD_FIRST(name, list, element) arraylist_add_first_##name(list, element)

// Add element at the end
#define ARRAYLIST_ADD_LAST(name, list, element) arraylist_add_last_##name(list, element)

// Remove element at specific index
#define ARRAYLIST_REMOVE(name, list, index, out) arraylist_remove_##name(list, index, out)

// Remove the first element
#define ARRAYLIST_REMOVE_FIRST(name, list, out) arraylist_remove_first_##name(list, out)

// Remove the last element
#define ARRAYLIST_REMOVE_LAST(name, list, out) arraylist_remove_last_##name(list, out)

#endif // ARRAYLIST_H
