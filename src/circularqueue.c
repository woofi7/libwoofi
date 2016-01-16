#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "woofi/circularqueue.h"
#ifdef WITH_TEST
# include <criterion/criterion.h>
#endif

circularqueue *circularqueue_create(size_t size) {
    circularqueue *queue = NULL;

    queue = malloc(sizeof(*queue));
    if(queue == NULL) {
	return NULL;
    }

    queue->requested_size = size;
    if(size == 0) {
        size = 100;
    }
    queue->size = size;
    queue->head = 0;
    queue->tail = 0;
    queue->element = calloc(queue->size, sizeof(*(queue->element)));

    return queue;
}

void circularqueue_delete(circularqueue *queue) {
    assert(queue);

    free(queue->element);
    free(queue);
}

static int circularqueue_grow(circularqueue *queue) {
    if (queue == NULL) {
        return -1;
    }

    int add = queue->size / 2;
    int tempo = queue->element[queue->head];

    queue->size = queue->size + add;

    int *new_elements = realloc(queue->element, queue->size* sizeof(*(queue->element)));
    if (new_elements == NULL) {
        return -1;
    }
    else {
        queue->element = new_elements;
    }

    if(queue->tail < queue->head) {
        for (int i = 1; i <= add; ++i) {
            queue->element[queue->head + i] = tempo;
            tempo = queue->element[queue->head + i];
        }

        queue->head += add;
    }

    return 0;
}

int circularqueue_is_empty(const circularqueue *queue) {
    assert(queue);

    return queue->tail == queue->head;
}

void circularqueue_print(const circularqueue *queue) {
    assert(queue);

    printf("[");
    for (size_t i = queue->head; i != queue->tail; ++i) {
	if (i == queue->size) {
	    i = 0;
	}
	printf("%d, ", queue->element[i]);
    }
    printf("]\n");
}


int circularqueue_insert(circularqueue *queue, int value) {
    assert(queue);

    int rc = 0;

    if (queue->tail >= queue->size) {
        queue->tail = 0;
    }

    if (queue->requested_size == 0) {
        if (queue->tail + 1 == queue->head) {
            rc = circularqueue_grow(queue);
            if(rc == -1) {
                return 0;
            }
        }
        else if (queue->tail == queue->size - 1 && queue->head == 0) {
            rc = circularqueue_grow(queue);
            if(rc == -1) {
                return 0;
            }
        }
    }

    queue->element[queue->tail] = value;
    queue->tail++;

    return 1;
}

int circularqueue_remove(circularqueue *queue) {
    assert(queue);

    if(circularqueue_is_empty(queue)) {
        return 0;
    }
    queue->head++;

    if (queue->head >= queue->size) {
	queue->head = 0;
    }

    return 1;
}

size_t circularqueue_count(const circularqueue *queue) {
    assert(queue);

    if(queue->tail < queue->head) {
        return queue->size - (queue->head - queue->tail);
    }
    else {
        return queue->tail - queue->head;
    }
}

int circularqueue_head(const circularqueue *queue) {
    assert(queue);

    return queue->element[queue->head];
}

#ifdef WITH_TEST
Test(CircularQueue, create) {
    circularqueue *queue = circularqueue_create(100);
    cr_assert(queue);
    cr_assert(circularqueue_is_empty(queue));
    cr_assert(circularqueue_count(queue) == 0);

    circularqueue_delete(queue);
}

Test(CircularQueue, insert) {
    int rc = 0;
    circularqueue *queue = circularqueue_create(100);

    rc = circularqueue_insert(queue, 42);
    cr_assert(rc);
    cr_assert(circularqueue_head(queue) == 42);

    rc = circularqueue_insert(queue, 24);
    cr_assert(rc);
    cr_assert(circularqueue_head(queue) == 42);

    circularqueue_remove(queue);
    cr_assert(circularqueue_head(queue) == 24);

    circularqueue_delete(queue);
}

Test(CircularQueue, remove) {
    int rc = 0;
    circularqueue *queue = circularqueue_create(100);

    rc = circularqueue_remove(queue);
    cr_assert_not(rc);

    circularqueue_insert(queue, 42);
    cr_assert(circularqueue_head(queue) == 42);
    circularqueue_insert(queue, 24);
    cr_assert(circularqueue_head(queue) == 42);
    circularqueue_insert(queue, 12);
    cr_assert(circularqueue_head(queue) == 42);
    rc = circularqueue_remove(queue);
    cr_assert(rc);
    cr_assert(circularqueue_head(queue) == 24);
    rc = circularqueue_remove(queue);
    cr_assert(rc);
    cr_assert(circularqueue_head(queue) == 12);
    rc = circularqueue_remove(queue);
    cr_assert(rc);

    circularqueue_delete(queue);
}

Test(CircularQueue, pushPop10000) {
    int rc = 0;
    circularqueue *queue = circularqueue_create(100);

    for (int i = 0; i < 10000; i++) {
	rc = circularqueue_insert(queue, i);
	cr_assert(rc, "Error when insert `%d` element", i);
	rc = circularqueue_head(queue);
	cr_assert(rc == i, "head should be `%d`, but was `%d`", i, rc);
	cr_assert(1 == circularqueue_count(queue));
	rc = circularqueue_remove(queue);
	cr_assert(rc, "Error when removing `%d` element", i);
    }

    circularqueue_delete(queue);
}

Test(CircularQueue, countOverflow) {
    int rc = 0;
    size_t count = 0;
    circularqueue *queue = circularqueue_create(100);

    for (size_t i = 0; i < queue->size - 1; i++) {
	count = circularqueue_count(queue);
	cr_assert(i == count, "Count should be %u but was %u", i,  count);
	rc = circularqueue_insert(queue, i);
	cr_assert(rc, "Error when insert `%u` element", i);
    }

    cr_assert(queue->size - 1 == circularqueue_count(queue));

    for (int i = 0; i < 50; i++) {
	rc = circularqueue_remove(queue);
	rc = circularqueue_insert(queue, i);
    }
    count = circularqueue_count(queue);
    cr_assert(queue->size - 1 == count
            , "Count should be %d but was %u"
            , queue->size - 1
            , count);

    circularqueue_delete(queue);
}

Test(CircularQueue, grow_head_before) {
    int rc = 0;
    circularqueue *queue = circularqueue_create(0);

    for(int i = 0; i < 99; i++){
        circularqueue_insert(queue, i);
        cr_assert(queue->size == 100);
    }

    rc = circularqueue_insert(queue, 42);
    cr_assert(rc);
    cr_assert(queue->size == 150);
    cr_assert(queue->head == 0);
    cr_assert(queue->tail == 100);

    circularqueue_delete(queue);
}

Test(CircularQueue, grow_head_after) {
    int rc;
    circularqueue *queue = circularqueue_create(0);

    for(int i = 0; i < 99; i++){
        circularqueue_insert(queue, i);
        cr_assert(queue->size == 100);
    }

    for(int i = 0; i < 50; i++){
        circularqueue_remove(queue);
    }

    for(int i = 0; i < 50; i++){
        circularqueue_insert(queue, i);
        cr_assert(queue->size == 100);
    }
    rc = circularqueue_insert(queue, 42);
    cr_assert(rc);
    cr_assert(queue->size == 150);
    cr_assert(queue->element[queue->head] == 50);
    cr_assert(queue->element[queue->tail - 1] == 42);

    circularqueue_delete(queue);
}

#endif
