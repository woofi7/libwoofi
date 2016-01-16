
typedef struct {
    int *element;
    size_t head;
    size_t tail;
    size_t size;
    size_t requested_size;
} circularqueue;

/**
 * Create a new circural queue
 * Must be free with circularlist_delete
 * @return A pointer to an allocated circular queue or NULL on error (see errno)
 */
circularqueue *circularqueue_create(size_t size);

/**
 * Free all used memory by the queue
 * @param queue a non null pointer to a queue
 */
void circularqueue_delete(circularqueue *queue);

/**
 * Check if the queue is empty or not
 * @param queue a non null pointer to a queue
 * @return 0 if the queue is not empty
 * 		   1 if it's empty
 */
int circularqueue_is_empty(const circularqueue *queue);

/**
 * Print the queue on STDOUT at format [X,Y,Z]
 * @param queue a non null pointer to a queue
 */
void circularqueue_print(const circularqueue *queue);

/**
 * Insert element at the end of the queue
 * @param queue a non null pointer to a queue
 * @param value the value to be added to the queue
 * @return 0 if the element was added to the queue
 * 		  -1 on error, if it failed to allocate requested memory (see errno)
 */
int circularqueue_insert(circularqueue *queue, int value);

/**
 * Remove value at the head of the queue
 * @param queue a non null pointer to a queue
 * @param value the value to be added to the queue
 * @return 1 if the element was deleted
 * 		   0 if the element can't be deleted
 */
int circularqueue_remove(circularqueue *queue);

/**
 * Count the number of element in the queue and return it
 * @param queue initialized @see circularqueue_create
 * @return the number of elements on the queue
 */
size_t circularqueue_count(const circularqueue *queue);

/**
 * Return the head of the file
 * @param queue a non null pointer to a queue
 * @return the head of the file
 */
int circularqueue_head(const circularqueue *queue);
