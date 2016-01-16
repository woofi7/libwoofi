#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "woofi/stack.h"
#ifdef WITH_TEST
# include <criterion/criterion.h>
#endif

stack *stack_create() {
    stack *stack = NULL;

    stack = malloc(sizeof(*stack));
    if(stack == NULL) {
        return NULL;
    }

    stack->size = INITIAL_STACK_SIZE;
    stack->head = 0;
    stack->element = calloc(stack->size, sizeof(*(stack->element)));

    return stack;
}

void stack_delete(stack *stack) {
    assert(stack);

    free(stack->element);
    free(stack);
}

static int stack_grow(stack *stack) {
    if (stack == NULL) {
        return -1;
    }

    stack->size = stack->size + stack->size / 2;

    int *new_elements = realloc(stack->element, stack->size* sizeof(*(stack->element)));
    if (new_elements == NULL) {
        return -1;
    }
    else {
        stack->element = new_elements;
    }

    return 0;
}

int stack_is_empty(const stack *stack) {
    assert(stack);

    return stack->head == 0;
}

size_t stack_count(const stack *stack) {
    return stack->head;
}

int stack_insert(stack *stack, int value) {
    assert(stack);

    int rc = 0;

    if (stack->head == stack->size) {
        rc = stack_grow(stack);
        if(rc == -1) {
            return 0;
        }
    }

    stack->head++;
    stack->element[stack->head] = value;

    return 1;
}

int stack_remove(stack *stack) {
    assert(stack);

    if(stack_is_empty(stack)) {
        return 0;
    }
    stack->head--;

    return 1;
}

int stack_head(const stack *stack) {
    return stack->element[stack->head];
}

#ifdef WITH_TEST
Test(Stack, create) {
    stack *stack = stack_create();

    cr_assert(stack);
    cr_assert(stack_is_empty(stack));
    cr_assert(stack_count(stack) == 0);
    cr_assert(stack->size == 100);

    stack_delete(stack);
}

Test(Stack, insert) {
    int rc = 0;
    stack *stack = stack_create();

    rc = stack_insert(stack, 42);
    cr_assert(rc);
    cr_assert(stack_head(stack) == 42);
    cr_assert(stack->head == 1);

    rc = stack_insert(stack, 24);
    cr_assert(rc);
    cr_assert(stack_head(stack) == 24);
    cr_assert(stack->head == 2);

    rc = stack_remove(stack);
    cr_assert(rc);
    cr_assert(stack_head(stack) == 42);
    cr_assert(stack->head == 1);

    stack_delete(stack);
}

Test(Stack, remove) {
    int rc = 0;
    stack *stack = stack_create();

    rc = stack_remove(stack);
    cr_assert_not(rc);

    rc = stack_insert(stack, 42);
    cr_assert(rc);
    cr_assert(stack_head(stack) == 42);
    rc = stack_remove(stack);
    cr_assert(rc);

    rc = stack_insert(stack, 24);
    cr_assert(rc);
    rc = stack_insert(stack, 12);
    cr_assert(rc);
    cr_assert(stack_head(stack) == 12);
    rc = stack_remove(stack);
    cr_assert(rc);
    cr_assert(stack_head(stack) == 24);

    stack_delete(stack);
}

Test(Stack, grow) {
    int rc = 0;
    stack *stack = stack_create();

    for(int i = 0; i < 100; i++) {
        rc = stack_insert(stack, i);
        cr_assert(rc);
    }

    cr_assert(stack->head == 100);
    cr_assert(stack->size == 100);
    rc = stack_insert(stack, 42);
    cr_assert(rc);
    cr_assert(stack->size == 150);

    stack_delete(stack);
}

Test(Stack, shit_load) {
    int rc = 0;
    size_t count = 1;
    stack *stack = stack_create();

    for(int i = 0; i < 20000; i++) {
        rc = stack_insert(stack, i);
        cr_assert(rc);
        cr_assert(stack->head == count);

        count++;
    }

    stack_delete(stack);
}

#endif
