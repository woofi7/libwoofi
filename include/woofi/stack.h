#define INITIAL_STACK_SIZE 100

typedef struct {
    size_t size;
    size_t head;
    int *element;
} stack;

/**
 * Create a new stack
 * Must be free with stack_delete
 * @return A pointer to an allocated stack or NULL on error (see errno)
 */
stack *stack_create();

/**
 * Free all used memory by the stack
 * @param stack a non null pointer to a stack
 */
void stack_delete(stack *stack);

/**
 * Check if the stack is empty or not
 * @param stack a non null pointer to a stack
 * @return 0 if the stack is not empty
 *         1 if it's empty
 */
int stack_is_empty(const stack *stack);

/**
 * Print the stack on STDOUT at format [X,Y,Z]
 * @param stack a non null pointer to a stack
 */
void stack_print(const stack *stack);

/**
 * Insert element at the head of the stack
 * @param stack a non null pointer to a stack
 * @param value the value to be added to the stack
 * @return 0 if the element was added to the stack
 *        -1 on error, if it failed to allocate requested memory (see errno)
 */
int stack_insert(stack *stack, int value);

/**
 * Remove value at the head of the stack
 * @param queue a non null pointer to a stack
 * @param value the value to be added to the stack
 * @return 1 if the element was deleted
 *         0 if the element can't be deleted
 */
int stack_remove(stack *stack);

/**
 * Count the number of element in the stack and return it
 * @param stack initialized @see stack_create
 * @return the number of elements on the stack
 */
size_t stack_count(const stack *stack);

/**
 * Return the head of the stack
 * @param stack a non null pointer to a stack
 * @return the head of the stack
 */
int stack_head(const stack *stack);
