
#define INITIALI_ARRAYLIST_SIZE 100

typedef struct {
    size_t length;
    size_t count;
    int *element;
} arraylist;

/**
 * Create a new array list.
 * Must be free with arraylist_delete
 * @return A pointer to an allocated array list or NULL on error (see errno)
 */
arraylist *arraylist_create();

/**
 * Free all used memory by the list
 * @param list a non null pointer to a list
 */
void arraylist_delete(arraylist *list);

/**
 * Check if the list is empty or not
 * @param list a non null pointer to a list
 * @return 0 if the list is not empty
 * 		   1 if it's empty
 */
int arraylist_is_empty(const arraylist *list);

/**
 * Print the list on STDOUT at format [X,Y,Z]
 * @param list a non null pointer to a list
 */
void arraylist_print(const arraylist *list);

/**
 * Insert element at the start of the list
 * @param list a non null pointer to a list
 * @param value the value to be added to the list
 * @return 0 if the element was added to the list
 * 		  -1 on error, if it failed to allocate requested memory (see errno)
 */
int arraylist_insert_front(arraylist *list, int value);

/**
 * Insert element at the end of the list
 * @param list a non null pointer to a list
 * @param value the value to be added to the list
 * @return 0 if the element was added to the list
 * 		  -1 on error, if it failed to allocate requested memory (see errno)
 */
int arraylist_insert_last(arraylist *list, int value);

/**
 * Insert element at the index of the list
 * @param list a non null pointer to a list
 * @param index the index of the requested element
 * @param value the value to be added to the list
 * @return 0 if the element was added to the list
 * 		  -1 on error, if it failed to allocate requested memory (see errno)
 */
int arraylist_insert_at(arraylist *list, size_t index, int value);

/**
 * Remove the value from the list
 * @param list a non null pointer to a list
 * @param value the value to be removed to the list
 * @return 1 if the element was deleted
 * 		   0 if the element can't be deleted
 */
int arraylist_remove_value(arraylist *list, int value);

/**
 * Remove the value at the index from the list
 * @param list a non null pointer to a list
 * @param index the index of the requested element
 * @return 1 if the element was deleted
 * 		   0 if the element can't be deleted
 */
int arraylist_remove_at(arraylist *list, size_t index);

/**
 * Check if the value is in list
 * @param list a non null pointer to a list
 * @param value the value to search on the list
 * @return 0 if the value is not in list
 *         1 if the value is in list
 */
int arraylist_contains(const arraylist *list, int value);

/**
 * Get the value at specified index in the list.
 * The index value MUST be valid (>= 0 AND < size)
 * @param list initialized @see arraylist_create
 * @param index the index of the requested element
 * @return the value of requested element
 */
int arraylist_fast_get(const arraylist *list, size_t index);

/**
 * Get the value at specified index in the list.
 * @param list initialized @see arraylist_create
 * @param index the index of the requested element
 * @param found a pointer to store the result of the search (if
 *  the element was found)
 * @return the value of requested element
 */
int arraylist_get(const arraylist *list, size_t index, int *found);

/**
 * Count the number of element in list and return it
 * @param list initialized @see arraylist_create
 * @return the number of elements on the list
 */
int arraylist_count(const arraylist *list);

